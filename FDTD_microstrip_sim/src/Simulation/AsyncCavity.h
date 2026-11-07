#pragma once
#include "CavityController.h"
#include "PhysicalPlayback.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Immutable display data: no magnetic fields or update coefficients copied.
struct CavityDisplayData
{
    CavityController::Settings settings;
    std::array<std::vector<float>,3> electric;
    std::array<std::vector<bool>,3> conductor;
    std::array<FieldProbe,2> probes;
    std::vector<ElectricDrive> sources;
    int steps=0;
    double dt=0, measured=0, analytical=0, stepMilliseconds=0;
    std::size_t bytes=0, conductorCount=0;
    std::uint64_t revision=0;
    bool ready=false;

    explicit CavityDisplayData(const CavityController& c) : settings(c.settings()), probes(c.probes()),
        sources(c.sources()), steps(c.stepCount()), dt(c.timestep()), measured(c.measuredFrequency()),
        analytical(c.analyticalFrequency()), bytes(c.allocatedBytes()), conductorCount(c.conductorSamples()),
        revision(c.fieldRevision()), ready(c.ready())
    {
        if(ready) for(int a=0;a<3;++a) {
            electric[a]=c.electricSamples(static_cast<Yee3D::Axis>(a));
            conductor[a]=c.conductorMask(static_cast<Yee3D::Axis>(a));
        }
    }
    CavityDisplayData() = default;
};

// UI-thread facade. The worker exclusively owns its mutable solver; rendering
// reads only front. Locks protect commands/publication, never an FDTD step.
class AsyncCavity
{
public:
    AsyncCavity() : front(std::make_shared<CavityDisplayData>()), worker(&AsyncCavity::work,this) {}
    ~AsyncCavity() { shutdown(); }
    AsyncCavity(const AsyncCavity&)=delete;
    AsyncCavity& operator=(const AsyncCavity&)=delete;

    void shutdown() {
        { std::lock_guard<std::mutex> lock(mutex); stopping=true; running=false; }
        changed.notify_all();
        if(worker.joinable()) worker.join();
    }
    // Mapping callbacks borrow scene objects only here, on the UI thread.
    bool reset(CavityController::Settings settings, const CavityController::MaterialSampler& material={},
        const Yee3D::ConductorSampler& conductors={}, const std::vector<ElectricDriveDefinition>& sources={})
    {
        setRunning(false);
        try {
            // Account for old/new solvers plus up to three immutable displays.
            const auto displayBytes=[](const GridSettings& g) { return g.electricCount()*sizeof(float)+(g.electricCount()+7)/8; };
            settings.grid.validate(allocatedBytes()+3*std::max(displayBytes(grid()),displayBytes(settings.grid)));
            auto next=std::make_unique<CavityController>();
            if(!next->reset(settings,material,conductors,sources)) { errorText=next->error(); return false; }
            auto display=std::make_shared<CavityDisplayData>(*next);
            { std::lock_guard<std::mutex> lock(mutex);
                ++generation; replacement=std::move(next); published.reset(); queuedSteps=0; probeChange=false;
                front=std::move(display); errorText.clear(); workerError.clear(); ++displayRevision;
                // Reference is a playback conversion only, not a source edit.
                playback.referenceHz=!front->sources.empty() ? front->sources.front().frequencyHz :
                    (front->analytical>0 ? front->analytical : 1e9);
                restartMeter();
            }
            changed.notify_all();
            return true;
        } catch(const std::exception& e) { errorText=e.what(); return false; }
    }
    void poll() {
        std::lock_guard<std::mutex> lock(mutex);
        if(published && publishedGeneration==generation) { front=std::move(published); ++displayRevision; }
        if(!workerError.empty()) errorText=workerError;
    }
    void setRunning(bool value) {
        { std::lock_guard<std::mutex> lock(mutex);
            const bool next=value && ready() && errorText.empty() && !stopping;
            if(next!=running) { running=next; restartMeter(); }
        }
        changed.notify_all();
    }
    bool isRunning() const { std::lock_guard<std::mutex> lock(mutex); return running; }
    void stepOnce() {
        { std::lock_guard<std::mutex> lock(mutex);
            running=false; restartMeter();
            if(ready() && errorText.empty() && !stopping && queuedSteps<1) ++queuedSteps;
        }
        changed.notify_all();
    }
    void setPlayback(const PhysicalPlayback& value) {
        value.stepsPerSecond(ready() ? timestep() : 1);
        { std::lock_guard<std::mutex> lock(mutex); playback=value; restartMeter(); }
        changed.notify_all();
    }
    PhysicalPlayback playbackSettings() const { std::lock_guard<std::mutex> lock(mutex); return playback; }
    double achievedStepsPerSecond() const {
        std::lock_guard<std::mutex> lock(mutex);
        const double seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-meterStart).count();
        return running && seconds>0 ? completedPlaybackSteps/seconds : 0;
    }
    double playbackElapsed() const {
        std::lock_guard<std::mutex> lock(mutex);
        return running ? std::chrono::duration<double>(std::chrono::steady_clock::now()-meterStart).count() : 0;
    }
    void configureProbes(Yee3D::Axis axis,Yee3D::Index first,Yee3D::Index second) {
        sample(first,axis); sample(second,axis); // validate against current display
        { std::lock_guard<std::mutex> lock(mutex); probeAxis=axis; probeFirst=first; probeSecond=second; probeChange=true; }
        changed.notify_all();
    }
    bool ready() const { return front->ready; }
    const std::string& error() const { return errorText; }
    const CavityController::Settings& settings() const { return front->settings; }
    const GridSettings& grid() const { return settings().grid; }
    Yee3D::Index cells() const { return grid().cells; }
    double spacing() const { return grid().spacing; }
    int stepCount() const { return front->steps; }
    double timestep() const { return front->dt; }
    double time() const { return stepCount()*timestep(); }
    double stepMilliseconds() const { return front->stepMilliseconds; }
    double measuredFrequency() const { return front->measured; }
    double analyticalFrequency() const { return front->analytical; }
    std::size_t allocatedBytes() const { return front->bytes; }
    std::size_t conductorSamples() const { return front->conductorCount; }
    std::uint64_t fieldRevision() const { return displayRevision; }
    const std::array<FieldProbe,2>& probes() const { return front->probes; }
    const std::vector<ElectricDrive>& sources() const { return front->sources; }
    float sample(Yee3D::Index p,Yee3D::Axis a=Yee3D::Z) const {
        const auto i=offset(p,a); return front->electric[a].at(i);
    }
    bool conductorAt(Yee3D::Index p,Yee3D::Axis a=Yee3D::Z) const {
        const auto i=offset(p,a); return front->conductor[a].at(i);
    }

private:
    // Called only with mutex held. A new schedule interrupts a long wait.
    void restartMeter() { ++schedule; completedPlaybackSteps=0; meterStart=std::chrono::steady_clock::now(); }
    std::size_t offset(Yee3D::Index p,Yee3D::Axis a) const {
        if(!ready() || a<Yee3D::X || a>Yee3D::Z) throw std::out_of_range("Invalid display sample");
        auto size=cells(); for(int i=0;i<3;++i) if(i!=a) ++size[i];
        for(int i=0;i<3;++i) if(p[i]>=size[i]) throw std::out_of_range("Display sample outside grid");
        return (p[2]*size[1]+p[1])*size[0]+p[0];
    }
    void work() {
        using Clock=std::chrono::steady_clock;
        std::unique_ptr<CavityController> solver;
        std::uint64_t solverGeneration=0;
        std::uint64_t workerSchedule=0;
        auto delayStart=Clock::now();
        double delaySeconds=0;
        std::unique_lock<std::mutex> lock(mutex);
        while(!stopping) {
            changed.wait(lock,[&] { return stopping || replacement || probeChange || queuedSteps || running; });
            if(stopping) break;
            if(replacement) { solver=std::move(replacement); solverGeneration=generation; delaySeconds=0; }
            if(workerSchedule!=schedule) { workerSchedule=schedule; delaySeconds=0; }
            if(!solver) continue;
            const auto now=Clock::now();
            delaySeconds=std::max(0.0,delaySeconds-std::chrono::duration<double>(now-delayStart).count());
            delayStart=now;
            if(running && !queuedSteps && !probeChange && delaySeconds>0) {
                // Bounded waits avoid chrono overflow for extremely slow requests;
                // unused delay is retained, not converted into an early step.
                changed.wait_for(lock,std::chrono::duration<double>(std::min(60.0,delaySeconds))); continue;
            }
            const bool doProbe=probeChange;
            const auto axis=probeAxis; const auto first=probeFirst, second=probeSecond;
            probeChange=false;
            const bool doStep=queuedSteps || (running && !doProbe);
            if(queuedSteps) --queuedSteps;
            const double targetRate=playback.stepsPerSecond(solver->timestep());
            const auto stepSchedule=schedule;
            const bool automatic=running && doStep;
            lock.unlock();
            try {
                const auto start=Clock::now();
                if(doProbe) solver->configureProbes(axis,first,second);
                if(doStep) solver->stepOnce();
                auto display=std::make_shared<CavityDisplayData>(*solver);
                const auto finished=Clock::now();
                display->stepMilliseconds=std::chrono::duration<double,std::milli>(finished-start).count();
                // At most 50% worker duty cycle, and no missed-step catch-up.
                const double workSeconds=std::chrono::duration<double>(finished-start).count();
                delaySeconds=std::max(workSeconds,1.0/targetRate-workSeconds);
                delayStart=finished;
                lock.lock();
                if(automatic && running && stepSchedule==schedule && solverGeneration==generation) ++completedPlaybackSteps;
                if(solverGeneration==generation) { published=std::move(display); publishedGeneration=solverGeneration; }
            } catch(const std::exception& e) {
                lock.lock();
                if(solverGeneration==generation) { workerError=e.what(); running=false; queuedSteps=0; }
            }
        }
    }
    std::shared_ptr<const CavityDisplayData> front, published;
    std::string errorText, workerError;
    mutable std::mutex mutex;
    std::condition_variable changed;
    bool stopping=false, running=false, probeChange=false;
    int queuedSteps=0;
    PhysicalPlayback playback;
    std::uint64_t schedule=0, completedPlaybackSteps=0;
    std::chrono::steady_clock::time_point meterStart=std::chrono::steady_clock::now();
    Yee3D::Axis probeAxis=Yee3D::Z;
    Yee3D::Index probeFirst{},probeSecond{};
    std::uint64_t generation=0, publishedGeneration=0, displayRevision=0;
    std::unique_ptr<CavityController> replacement;
    std::thread worker; // constructed last; joined before any state is destroyed
};
