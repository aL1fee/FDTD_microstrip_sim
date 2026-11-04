#include "../src/Simulation/CavityController.h"
#include <cstdio>
#include <limits>
static void require(bool ok) { if (!ok) throw std::runtime_error("Grid settings regression"); }
template<class F> void rejects(F f) { bool rejected=false; try { f(); } catch(const std::invalid_argument&) { rejected=true; } require(rejected); }
int main()
{
    auto g = GridSettings::fromDomain({.0045,.002,.007}, static_cast<double>(.02f)*1e-3);
    require(g.cells == Yee3D::Index{225,100,350});
    rejects([&] { g.validate(); }); // includes room for an atomic replacement
    const auto screenshotGrid=GridSettings::fromDomain({4.5e-3,2e-3,7e-3},static_cast<double>(.15f)*1e-3);
    require(screenshotGrid.cells==Yee3D::Index{30,13,46});
    auto rounded = GridSettings::fromDomain({.0046,.0022,.0031}, .0005);
    require(rounded.cells == Yee3D::Index{9,4,6});
    rejects([] { GridSettings::fromDomain({1,1,1}, 0); });
    rejects([] { GridSettings::fromDomain({.001,.001,.001}, .001); });
    rejects([] { GridSettings::fromDomain({1,1,1}, std::numeric_limits<double>::quiet_NaN()); });
    rejects([] { GridSettings::fromDomain({1,1,1}, .00001); });
    CavityController c;
    for (auto grid : {GridSettings{{8,6,4},.00025}, GridSettings{{3,5,2},.001}})
    {
        CavityController::Settings settings{CavityController::Material::Vacuum,1,grid};
        require(c.reset(settings));
        require(c.cells()==grid.cells && c.spacing()==grid.spacing);
        Yee3D reference(grid.cells, grid.spacing);
        const auto n=grid.cells;
        for(std::size_t z=0;z<n[2];++z) for(std::size_t y=1;y<n[1];++y) for(std::size_t x=1;x<n[0];++x)
            reference.injectElectric(Yee3D::Z,{x,y,z},static_cast<float>(std::sin(std::acos(-1.0)*x/n[0])*std::sin(std::acos(-1.0)*y/n[1])));
        for(int step=0;step<100;++step)
        {
            c.stepOnce(); reference.step();
            for(std::size_t z=0;z<n[2];++z) for(std::size_t y=0;y<=n[1];++y) for(std::size_t x=0;x<=n[0];++x)
                require(c.sample({x,y,z})==reference.sampleElectric(Yee3D::Z,{x,y,z}));
        }
        require(std::abs(c.time()-100*reference.timestep())<1e-20);
        const auto before=c.sample({1,1,1});
        require(!c.reset({CavityController::Material::Scene,1,g}));
        require(c.cells()==n && c.stepCount()==100 && c.sample({1,1,1})==before);
        require(!c.isRunning());
    }
    require(c.reset({}));
    require(c.cells()==Yee3D::Index{24,20,16} && c.spacing()==.0005);
    std::puts("Grid conversion, limits, arbitrary dimensions, exact stepping, failed-reset preservation and preset restoration passed.");
}
