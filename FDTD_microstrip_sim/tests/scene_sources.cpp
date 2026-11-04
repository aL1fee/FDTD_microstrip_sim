#include "../src/Simulation/CavityController.h"
#include "../src/Simulation/FieldSlice.h"
#include <cstdio>
static void require(bool ok) { if(!ok) throw std::runtime_error("Scene source regression"); }
int main()
{
    CavityController::Settings settings{CavityController::Material::Uniform,1,GridSettings{{16,12,10},.0005},CavityController::Excitation::SceneSources};
    for(auto axis : {Yee3D::X,Yee3D::Y,Yee3D::Z})
    {
        const Yee3D::Index driven{4,5,4};
        ElectricDriveDefinition source{7,axis,10e9,1,[=](const auto& p){
            for(int a=0;a<3;++a) if(std::abs(p[a]-(driven[a]+(a==axis ? .5:0))*.0005)>1e-10) return false;
            return true;
        }};
        CavityController c;
        require(c.reset(settings,{}, {}, {source}));
        require(c.sources().size()==1 && c.sources()[0].samples.size()==1);
        for(int a=0;a<3;++a) require(c.sample({4,5,4},static_cast<Yee3D::Axis>(a))==0);
        const double dt=.5*.0005/Yee3D::vacuumSpeed;
        const double t=.5*dt, phase=2*std::acos(-1.0)*source.frequencyHz*t;
        const double expected=dt*2*std::acos(-1.0)*source.frequencyHz*.5*(1-std::cos(std::acos(-1.0)*t*source.frequencyHz/2))*std::sin(phase);
        c.stepOnce(); require(std::abs(c.sample(driven,axis)-expected)<1e-10);
        require(c.sample({10,5,4},axis)==0); // no pre-existing cavity mode
        const auto unitFirst=c.sample(driven,axis);
        auto dielectric=settings; dielectric.permittivity=4;
        require(c.reset(dielectric,{}, {}, {source})); c.stepOnce();
        require(std::abs(c.sample(driven,axis)-unitFirst/4)<1e-10);
        auto twice=source; twice.amplitude=2;
        require(c.reset(settings,{}, {}, {twice})); c.stepOnce();
        require(std::abs(c.sample(driven,axis)-2*unitFirst)<1e-10);
        require(c.reset(settings,{}, {}, {source,source})); c.stepOnce();
        require(std::abs(c.sample(driven,axis)-2*unitFirst)<1e-10);
        require(c.reset(settings,{}, {}, {source}));
        CavityController reference; require(reference.reset(settings,{}, {}, {source}));
        for(int n=0;n<120;++n) { c.stepOnce(); reference.setRunning(true); reference.update(1.0/80); }
        require(c.sample(driven,axis)==reference.sample(driven,axis));
        auto remote=driven; remote[(axis+1)%3]+=3;
        require(std::abs(c.sample(remote,axis))>1e-7); // injected energy reaches other cells
        const auto saved=c.sample(driven,axis);
        require(!c.reset(settings,{}, {}, {})); require(c.stepCount()==120 && c.sample(driven,axis)==saved);
        auto outside=source; outside.contains=[](const auto&){return false;};
        require(!c.reset(settings,{}, {}, {outside}));
        require(!c.reset(settings,{}, [](const auto&){return true;}, {source}));
        auto invalid=source; invalid.frequencyHz=1e15;
        require(!c.reset(settings,{}, {}, {invalid}));
        invalid=source; invalid.amplitude=-1; require(!c.reset(settings,{}, {}, {invalid}));
        require(c.reset(settings,{}, {}, {source})); require(c.sample(driven,axis)==0);
        // Source definitions are snapshots; mutations after reset cannot change the drive.
        source.amplitude=0; source.contains={}; c.stepOnce(); require(c.sample(driven,axis)>0);
        for(auto plane : {FieldSlice::XY,FieldSlice::XZ,FieldSlice::YZ})
        {
            FieldSlice slice(c.cells(),plane,3,axis);
            for(std::size_t v=0;v<slice.height();++v) for(std::size_t u=0;u<slice.width();++u)
                require(std::isfinite(c.sample(slice.index(u,v),axis)));
        }
    }
    // Validate the actual CW drive rate's periodic frequency after its two-cycle ramp.
    ElectricDrive waveform{0,Yee3D::Y,2e9,1,{}};
    const double period=1/waveform.frequencyHz;
    require(std::abs(waveform.rate(2.25*period)-waveform.rate(3.25*period))<1e-4);
    require(waveform.rate(2.25*period)>0 && waveform.rate(2.75*period)<0);
    std::puts("Scene sources: zero initial fields, midpoint timing, CW frequency, dielectric scaling, linearity, pause/step, PEC rejection and component slices passed.");
}
