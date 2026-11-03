#pragma once

#include "Yee3D.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <string>

class CavityController
{
public:
    enum class Material { Vacuum, Uniform, Scene };
    struct Settings
    {
        Material material = Material::Vacuum;
        float permittivity = 4.0f;
    };
    using MaterialSampler = std::function<double(const std::array<double, 3>&)>;

    static Yee3D::Index cells() { return {24, 20, 16}; }
    static double spacing() { return 0.5e-3; }

    bool reset(Settings settings, const MaterialSampler& sceneMaterial = {},
               const Yee3D::ConductorSampler& conductors = {})
    {
        running = false;
        try
        {
            auto next = std::make_unique<Yee3D>(cells(), spacing());
            if (settings.material == Material::Uniform)
                next->setPermittivity([&](const auto&) { return settings.permittivity; });
            if (settings.material == Material::Scene)
            {
                if (!sceneMaterial)
                    throw std::invalid_argument("Scene material sampler is required.");
                next->setPermittivity(sceneMaterial);
            }
            if (conductors) next->setConductors(conductors);
            const auto count = cells();
            const double pi = std::acos(-1.0);
            for (std::size_t z = 0; z < count[2]; ++z)
                for (std::size_t y = 1; y < count[1]; ++y)
                    for (std::size_t x = 1; x < count[0]; ++x)
                        next->injectElectric(Yee3D::Z, {x,y,z}, static_cast<float>(
                            std::sin(pi*x/count[0]) * std::sin(pi*y/count[1])));
            field = std::move(next);
            active = settings;
            steps = 0;
            budget = 0;
            crossings = 0;
            firstCrossing = lastCrossing = 0;
            previous = field->sampleElectric(Yee3D::Z, {12,10,8});
            errorText.clear();
            return true;
        }
        catch (const std::invalid_argument& error)
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

    bool ready() const { return static_cast<bool>(field); }
    bool isRunning() const { return running; }
    std::size_t conductorSamples() const { return field ? field->conductorSamples() : 0; }
    int stepCount() const { return steps; }
    double time() const { return field ? steps * field->timestep() : 0; }
    const std::string& error() const { return errorText; }
    double measuredFrequency() const
    {
        return crossings >= 2 ? (crossings - 1) / (lastCrossing - firstCrossing) : 0;
    }
    double analyticalFrequency() const
    {
        if (active.material == Material::Scene) return 0;
        const double epsilon = active.material == Material::Uniform ? active.permittivity : 1;
        return Yee3D::vacuumSpeed / 2 * std::hypot(1.0/0.012, 1.0/0.010) / std::sqrt(epsilon);
    }
    float sample(Yee3D::Index index) const { return field->sampleElectric(Yee3D::Z, index); }

private:
    void advance()
    {
        field->step();
        ++steps;
        const double value = field->sampleElectric(Yee3D::Z, {12,10,8});
        if (previous < 0 && value >= 0)
        {
            lastCrossing = (steps - 1 - previous / (value - previous)) * field->timestep();
            if (crossings++ == 0) firstCrossing = lastCrossing;
        }
        previous = value;
    }

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
