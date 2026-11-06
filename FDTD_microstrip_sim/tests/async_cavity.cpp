#include "../src/Simulation/AsyncCavity.h"
#include <cstdio>
#include <stdexcept>

static void require(bool ok) { if(!ok) throw std::runtime_error("Async cavity regression"); }
template<class Predicate> void waitFor(AsyncCavity& c, Predicate done) {
    const auto end=std::chrono::steady_clock::now()+std::chrono::seconds(10);
    do {
        c.poll(); if(done()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while(std::chrono::steady_clock::now()<end);
    throw std::runtime_error("Worker timed out");
}
int main() {
    AsyncCavity async;
    CavityController direct;
    CavityController::Settings settings;
    settings.grid={{20,12,8},.0002};
    settings.excitation=CavityController::Excitation::SceneSources;
    std::vector<ElectricDriveDefinition> sources{{1,Yee3D::Y,1e9,1,
        [](const std::array<double,3>& p) { return p[0]>.001 && p[0]<.0015 && p[1]>.0005 && p[1]<.001 && p[2]>.0005 && p[2]<.001; }}};
    require(async.reset(settings,{},{},sources));
    require(direct.reset(settings,{},{},sources));
    for(int n=1;n<=40;++n) {
        async.stepOnce(); direct.stepOnce();
        waitFor(async,[&] { return async.stepCount()==n; });
        require(!async.isRunning() && async.time()==direct.time());
        for(int axis=0;axis<3;++axis) {
            auto size=direct.cells(); for(int i=0;i<3;++i) if(i!=axis) ++size[i];
            for(std::size_t z=0;z<size[2];++z) for(std::size_t y=0;y<size[1];++y) for(std::size_t x=0;x<size[0];++x)
                require(async.sample({x,y,z},static_cast<Yee3D::Axis>(axis))==direct.sample({x,y,z},static_cast<Yee3D::Axis>(axis)));
        }
        require(async.probes()[0].lastStep==n && async.probes()[1].lastStep==n);
    }
    async.configureProbes(Yee3D::X,{2,3,4},{5,3,4});
    waitFor(async,[&] { return async.probes()[0].component==Yee3D::X; });
    require(async.stepCount()==40 && async.probes()[0].count==1);
    async.setStepRate(20); async.setRunning(true);
    const auto frozenStep=async.stepCount(); const auto frozenSample=async.sample({6,4,4},Yee3D::Y);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    // No poll: worker must not mutate the display being read by a frame.
    require(async.stepCount()==frozenStep && async.sample({6,4,4},Yee3D::Y)==frozenSample);
    waitFor(async,[&] { return async.stepCount()>frozenStep; });
    async.setRunning(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(80)); async.poll();
    const auto paused=async.stepCount();
    std::this_thread::sleep_for(std::chrono::milliseconds(80)); async.poll();
    require(async.stepCount()==paused);
    // Replacement generations discard any old in-flight publication.
    async.setRunning(true);
    CavityController::Settings replacement; replacement.grid={{7,6,5},.001};
    require(async.reset(replacement));
    std::this_thread::sleep_for(std::chrono::milliseconds(80)); async.poll();
    require(async.cells()==replacement.grid.cells && async.stepCount()==0 && !async.isRunning());
    require(!async.reset({CavityController::Material::Uniform,0}));
    require(!async.error().empty() && async.cells()==replacement.grid.cells && async.stepCount()==0);
    require(async.reset(replacement)); async.stepOnce();
    waitFor(async,[&] { return async.stepCount()==1; });
    async.setRunning(true); async.shutdown(); async.shutdown();
    require(!async.isRunning());
    std::puts("Worker exact fields/probes, immutable display, pause, reset generations, failed reset and shutdown passed.");
}
