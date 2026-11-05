#include "../src/Simulation/PropagationValidation.h"
#include <cstdio>
static void require(bool ok) { if(!ok) throw std::runtime_error("Field probe regression"); }
int main()
{
    try {
    FieldProbe ring;
    for(int n=0;n<3000;++n)ring.record(n,static_cast<float>(n));
    require(ring.count==2048 && ring.firstStep()==952 && ring.at(0)==952 && ring.at(2047)==2999);
    CavityController c; require(c.reset({}));
    require(c.probes()[0].count==1);
    c.update(1); require(c.probes()[0].count==1);
    c.stepOnce(); require(c.probes()[0].count==2 && c.probes()[0].lastStep==1);
    require(c.probes()[0].at(1)==c.sample(c.probes()[0].index));
    c.configureProbes(Yee3D::Y,{2,3,4},{4,3,4}); require(c.probes()[0].count==1 && c.probes()[0].firstStep()==1);
    try { c.configureProbes(Yee3D::Y,{2,3,4},{999,3,4}); require(false); } catch(const std::out_of_range&) {}
    require(c.probes()[1].index==Yee3D::Index{4,3,4});
    require(c.reset({})); require(c.probes()[0].count==1 && c.probes()[0].lastStep==0);
    PropagationValidation check; check.start();
    while(check.running()) check.advance(100);
    if(!check.error().empty()) throw std::runtime_error(check.error());
    require(check.completed().size()==4);
    for(const auto& r : check.completed()) {
        std::printf("eps %.0f dx %.0f um: delay %.6f ps expected %.6f ps, error %.4f%%\n",r.epsilon,r.spacing*1e6,r.measuredDelay*1e12,r.expectedDelay*1e12,100*r.relativeError);
        require(r.passed);
    }
    require(check.completed()[1].relativeError<check.completed()[0].relativeError);
    require(check.completed()[3].relativeError<check.completed()[2].relativeError);
    check.start(); check.advance(2); check.cancel(); require(!check.running());
    std::puts("Probe recording, wrapping, pause/reset, propagation speed and refinement checks passed.");
    } catch(const std::exception& e) { std::fprintf(stderr,"%s\n",e.what()); return 1; }
}
