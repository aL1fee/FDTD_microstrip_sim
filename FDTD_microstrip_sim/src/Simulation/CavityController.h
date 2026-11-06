#pragma once

#include "GridSettings.h"
#include "ElectricDrive.h"
#include "FieldProbes.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <cstdint>

class CavityController
{
public:
    enum class Material { Vacuum, Uniform, Scene };
    enum class Excitation { StandingWave, XPulse, SceneSources };
    struct Settings
    {
        Material material = Material::Vacuum;
        float permittivity = 4.0f;
        GridSettings grid;
        Excitation excitation = Excitation::StandingWave;
    };
    using MaterialSampler = std::function<double(const std::array<double, 3>&)>;

    Yee3D::Index cells() const { return active.grid.cells; }
    double spacing() const { return active.grid.spacing; }
    const Settings& settings() const { return active; }
    const GridSettings& grid() const { return active.grid; }
    std::size_t allocatedBytes() const {
        std::size_t bytes=ready() ? active.grid.residentBytes() : 0;
        for(const auto& drive : drives) bytes+=drive.samples.capacity()*sizeof(Yee3D::Index);
        return bytes;
    }
    const std::vector<ElectricDrive>& sources() const { return drives; }

    bool reset(Settings settings, const MaterialSampler& sceneMaterial = {},
               const Yee3D::ConductorSampler& conductors = {},
               const std::vector<ElectricDriveDefinition>& sourceDefinitions = {})
    {
        running = false;
        try
        {
            settings.grid.validate(allocatedBytes());
            auto next = std::make_unique<Yee3D>(settings.grid.cells, settings.grid.spacing);
            if (settings.material == Material::Uniform)
                next->setPermittivity([&](const auto&) { return settings.permittivity; });
            if (settings.material == Material::Scene)
            {
                if (!sceneMaterial)
                    throw std::invalid_argument("Scene material sampler is required.");
                next->setPermittivity(sceneMaterial);
            }
            if (conductors) next->setConductors(conductors);
            std::vector<ElectricDrive> nextDrives;
            std::size_t mappingBytes=0;
            if(settings.excitation == Excitation::SceneSources)
            {
                if(sourceDefinitions.empty()) throw std::invalid_argument("Scene sources requires at least one Source object.");
                for(const auto& source : sourceDefinitions)
                {
                    const auto label=std::string("Source ")+std::to_string(source.id)+": ";
                    if(source.axis<Yee3D::X || source.axis>Yee3D::Z || !source.contains ||
                       !std::isfinite(source.frequencyHz) || source.frequencyHz<=0 ||
                       !std::isfinite(source.amplitude) || source.amplitude<0 || source.amplitude>100)
                        throw std::invalid_argument(label+"invalid polarization, frequency, or amplitude (0..100).");
                    if(source.frequencyHz*next->timestep()>.05)
                        throw std::invalid_argument(label+"frequency requires at least 20 timesteps per cycle. Refine the grid or lower frequency.");
                    ElectricDrive drive{source.id,source.axis,source.frequencyHz,source.amplitude,{}};
                    auto size=settings.grid.cells;
                    for(int a=0;a<3;++a) if(a!=source.axis) ++size[a];
                    for(std::size_t z=0;z<size[2];++z) for(std::size_t y=0;y<size[1];++y) for(std::size_t x=0;x<size[0];++x)
                    {
                        Yee3D::Index index{x,y,z};
                        std::array<double,3> p{x*settings.grid.spacing,y*settings.grid.spacing,z*settings.grid.spacing};
                        p[source.axis]+=.5*settings.grid.spacing;
                        if(!source.contains(p)) continue;
                        bool wall=false;
                        for(int a=0;a<3;++a) if(a!=source.axis && (index[a]==0 || index[a]==size[a]-1)) wall=true;
                        if(wall || next->isConductor(source.axis,index)) { ++drive.excluded; continue; }
                        // Bound vector growth conservatively, including reset replacement storage.
                        mappingBytes+=4*sizeof(Yee3D::Index);
                        if(settings.grid.resetBytes()+std::max(allocatedBytes(),settings.grid.residentBytes())+mappingBytes>settings.grid.memoryBudget())
                            throw std::invalid_argument("Source mapping exceeds the solver memory budget.");
                        drive.samples.push_back(index);
                    }
                    if(drive.samples.empty()) throw std::invalid_argument(label+"no drivable samples. Check size, position, grid resolution, polarization and PEC overlap.");
                    nextDrives.push_back(std::move(drive));
                }
            }
            const auto count = settings.grid.cells;
            const double pi = std::acos(-1.0);
            if(settings.excitation != Excitation::SceneSources)
            for (std::size_t z = 0; z < count[2]; ++z)
                for (std::size_t y = 1; y < count[1]; ++y)
                    for (std::size_t x = 1; x < count[0]; ++x)
                    {
                        const double u = (static_cast<double>(x)-count[0]/4.0) / std::max(1.5, count[0]/40.0);
                        const double profile = settings.excitation == Excitation::StandingWave
                            ? std::sin(pi*x/count[0]) : std::exp(-0.5*u*u);
                        next->injectElectric(Yee3D::Z, {x,y,z}, static_cast<float>(profile * std::sin(pi*y/count[1])));
                    }
            mappedConductorCount = next->conductorSamples();
            field = std::move(next);
            drives = std::move(nextDrives);
            active = settings;
            steps = 0;
            budget = 0;
            crossings = 0;
            firstCrossing = lastCrossing = 0;
            previous = field->sampleElectric(Yee3D::Z, probe());
            configureProbes(drives.empty() ? Yee3D::Z : drives.front().axis,
                {count[0]/3,count[1]/2,count[2]/2}, {2*count[0]/3,count[1]/2,count[2]/2});
            ++revision;
            errorText.clear();
            return true;
        }
        catch (const std::exception& error)
        {
            errorText = error.what();
            return false;
        }
    }

    void setRunning(bool value)
    {
        running = value && field && errorText.empty();
        budget = 0;
    }

    void stepOnce()
    {
        setRunning(false);
        if (field && errorText.empty()) advance();
    }

    // Caller supplies elapsed wall time. No window or graphics dependencies.
    void update(double elapsedSeconds)
    {
        if (!running || !std::isfinite(elapsedSeconds) || elapsedSeconds <= 0) return;
        budget += std::min(elapsedSeconds, 0.1) * 80.0;
        const int due = static_cast<int>(budget);
        budget -= due;
        for (int i = 0; i < due; ++i) advance();
    }

    const std::array<FieldProbe,2>& probes() const { return fieldProbes; }
    double timestep() const { return field ? field->timestep() : 0; }
    void configureProbes(Yee3D::Axis axis,Yee3D::Index first,Yee3D::Index second)
    {
        if(!field || axis<Yee3D::X || axis>Yee3D::Z) throw std::invalid_argument("Invalid probe component");
        // Validate both before changing either history.
        field->sampleElectric(axis,first); field->sampleElectric(axis,second);
        fieldProbes[0].index=first; fieldProbes[1].index=second;
        for(auto& p : fieldProbes) { p.component=axis; p.clear(); p.record(steps,field->sampleElectric(axis,p.index)); }
    }
    std::uint64_t fieldRevision() const { return revision; }
    const std::vector<float>& electricSamples(Yee3D::Axis axis) const { return field->electricSamples(axis); }
    const std::vector<bool>& conductorMask(Yee3D::Axis axis) const { return field->conductorMask(axis); }
    bool ready() const { return static_cast<bool>(field); }
    bool isRunning() const { return running; }
    std::size_t conductorSamples() const { return mappedConductorCount; }
    int stepCount() const { return steps; }
    double time() const { return field ? steps * field->timestep() : 0; }
    const std::string& error() const { return errorText; }
    double measuredFrequency() const
    {
        return active.excitation == Excitation::StandingWave && crossings >= 2 ? (crossings - 1) / (lastCrossing - firstCrossing) : 0;
    }
    double analyticalFrequency() const
    {
        if (active.material == Material::Scene || active.excitation != Excitation::StandingWave) return 0;
        const double epsilon = active.material == Material::Uniform ? active.permittivity : 1;
        return Yee3D::vacuumSpeed / 2 * std::hypot(1.0/(cells()[0]*spacing()), 1.0/(cells()[1]*spacing())) / std::sqrt(epsilon);
    }
    bool conductorAt(Yee3D::Index index, Yee3D::Axis axis=Yee3D::Z) const { return field->isConductor(axis, index); }
    float sample(Yee3D::Index index, Yee3D::Axis axis=Yee3D::Z) const { return field->sampleElectric(axis, index); }

private:
    Yee3D::Index probe() const { const auto n=cells(); return {n[0]/2,n[1]/2,n[2]/2}; }
    void advance()
    {
        field->step();
        for(const auto& drive : drives)
        {
            const double rate=drive.rate((steps+.5)*field->timestep());
            for(const auto& index : drive.samples) field->driveElectric(drive.axis,index,rate);
        }
        ++steps;
        ++revision;
        for(auto& p : fieldProbes) p.record(steps,field->sampleElectric(p.component,p.index));
        const double value = field->sampleElectric(Yee3D::Z, probe());
        if (previous < 0 && value >= 0)
        {
            lastCrossing = (steps - 1 - previous / (value - previous)) * field->timestep();
            if (crossings++ == 0) firstCrossing = lastCrossing;
        }
        previous = value;
    }

    std::uint64_t revision=0;
    std::size_t mappedConductorCount=0;
    std::array<FieldProbe,2> fieldProbes;
    std::vector<ElectricDrive> drives;
    std::unique_ptr<Yee3D> field;
    Settings active;
    bool running = false;
    int steps = 0;
    double budget = 0;
    double previous = 0;
    int crossings = 0;
    double firstCrossing = 0, lastCrossing = 0;
    std::string errorText;
};
