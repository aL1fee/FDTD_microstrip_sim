#include "../src/Simulation/FieldSlice.h"
#include "../src/Simulation/ObservationPlane.h"
#include "../src/Simulation/CavityController.h"
#include <cstdio>
static void require(bool ok) { if(!ok) throw std::runtime_error("Field view / pulse regression"); }
int main()
{
    CavityController c;
    CavityController::Settings settings{CavityController::Material::Vacuum,1,GridSettings{{7,5,3},.001}};
    require(c.reset(settings));
    FieldSlice yz(c.cells(),FieldSlice::YZ,2);
    require(yz.width()==3 && yz.height()==6);
    require(yz.index(0,0)==Yee3D::Index{2,0,0}); // low Y is the bottom row
    require(yz.index(2,5)==Yee3D::Index{2,5,2}); // +Z right, +Y up
    for(auto plane : {FieldSlice::XY,FieldSlice::XZ,FieldSlice::YZ})
    {
        FieldSlice first(c.cells(),plane,-10), last(c.cells(),plane,10000);
        require(first.layer==0 && last.layer==last.samples[last.normal]-1);
        for(int layer=0;layer<static_cast<int>(last.samples[last.normal]);++layer)
        {
            FieldSlice s(c.cells(),plane,layer);
            for(std::size_t v=0;v<s.height();++v) for(std::size_t u=0;u<s.width();++u)
            {
                auto p=s.index(u,v);
                require(p[s.horizontal]==u && p[s.vertical]==v && p[s.normal]==static_cast<std::size_t>(layer));
                require(std::isfinite(c.sample(p)) && !c.conductorAt(p));
            }
        }
        require(last.position(.001) == (last.layer+(last.normal==2 ? .5 : 0))*.001);
    }
    for (auto orientation : {FieldSlice::XY,FieldSlice::XZ,FieldSlice::YZ})
    {
        ObservationPlane plane{settings.grid,orientation,1000,true,.2f};
        FieldSlice slice(settings.grid.cells,orientation,1000);
        const auto corners=plane.cornersMeters();
        for(const auto& point : corners) require(point[slice.normal]==slice.position(settings.grid.spacing));
        require(corners[0][slice.horizontal]==0 && corners[0][slice.vertical]==0);
        require(corners[2][slice.horizontal]==settings.grid.cells[slice.horizontal]*settings.grid.spacing);
        require(corners[2][slice.vertical]==settings.grid.cells[slice.vertical]*settings.grid.spacing);
    }
    for(float epsilon : {1.f,4.f})
    {
        settings={CavityController::Material::Uniform,epsilon,GridSettings{{160,128,2},.0005},CavityController::Excitation::XPulse};
        require(c.reset(settings));
        require(c.sample({40,64,1})==1 && c.sample({80,64,1})<1e-10);
        for(int step=0;step<40;++step) c.stepOnce();
        std::size_t peak=41; float amplitude=0;
        for(std::size_t x=41;x<100;++x) if(c.sample({x,64,1})>amplitude) { peak=x; amplitude=c.sample({x,64,1}); }
        const double expected=40+20/std::sqrt(epsilon);
        std::printf("X pulse eps %.0f: right peak %zu, expected %.1f, amplitude %.4f\n",epsilon,peak,expected,amplitude);
        require(std::abs(static_cast<double>(peak)-expected)<=2 && amplitude>.35f);
        require(std::abs(c.sample({80-peak,64,1})-amplitude)<1e-4);
        require(c.analyticalFrequency()==0 && c.measuredFrequency()==0);
    }
    settings.material=CavityController::Material::Scene;
    require(c.reset(settings,[](const auto&){ return 1.; },[](const auto&p){ return p[0]>.019 && p[0]<.021; }));
    require(c.sample({40,64,1})==0 && c.conductorAt({40,64,1}));
    for(int step=0;step<30;++step)c.stepOnce();
    require(c.sample({40,64,1})==0);
    require(c.reset({}));
    require(c.sample({12,10,8})==1 && c.analyticalFrequency()>0);
    std::puts("All slice bounds, staggered positions, bidirectional pulse travel and PEC masking passed.");
}
