#include "../src/Simulation/CavityController.h"
#include <cstdio>
#include <stdexcept>

void require(bool result)
{
    if (!result) throw std::runtime_error("Controller regression failed");
}

int main()
{
    for (float epsilon : {1.0f, 4.0f})
    {
        CavityController controller;
        const CavityController::Settings settings{CavityController::Material::Uniform, epsilon};
        require(controller.reset(settings));
        Yee3D reference({24,20,16}, 0.5e-3);
        reference.setPermittivity([&](const auto&) { return epsilon; });
        const double pi = std::acos(-1.0);
        for (std::size_t z=0; z<16; ++z)
            for (std::size_t y=1; y<20; ++y)
                for (std::size_t x=1; x<24; ++x)
                    reference.injectElectric(Yee3D::Z, {x,y,z}, static_cast<float>(
                        std::sin(pi*x/24) * std::sin(pi*y/20)));
        controller.setRunning(true);
        for (int n=0; n<1200; ++n)
        {
            reference.step();
            controller.update(1.0/80);
            for (std::size_t z=0; z<16; ++z)
                for (std::size_t y=0; y<=20; ++y)
                    for (std::size_t x=0; x<=24; ++x)
                        require(controller.sample({x,y,z}) == reference.sampleElectric(Yee3D::Z,{x,y,z}));
        }
        require(controller.stepCount()==1200);
        require(std::abs(controller.measuredFrequency()/controller.analyticalFrequency()-1)<0.002);
        controller.setRunning(false);
        controller.update(1);
        require(controller.stepCount()==1200);
        controller.stepOnce();
        require(controller.stepCount()==1201 && !controller.isRunning());
        require(!controller.reset({CavityController::Material::Uniform,0}));
        require(!controller.error().empty() && controller.stepCount()==1201);
        controller.setRunning(true);
        require(!controller.isRunning());
        require(controller.reset(settings));
        require(controller.stepCount()==0 && controller.time()==0 && controller.measuredFrequency()==0);
    }
    CavityController scene;
    require(scene.reset({CavityController::Material::Scene, 4},
        [](const auto&) { return 4.0; },
        [](const auto& p) { return p[0] > 0.004 && p[0] < 0.008; }));
    require(scene.conductorSamples() > 0);
    for (int step = 0; step < 100; ++step)
    {
        scene.stepOnce();
        require(scene.sample({12,10,8}) == 0);
    }
    require(scene.reset({CavityController::Material::Vacuum, 1}));
    require(scene.conductorSamples() == 0 && scene.sample({12,10,8}) == 1);
    std::puts("Controller matches legacy initialization and stepping exactly; pause, step, reset and failed setup passed.");
}
