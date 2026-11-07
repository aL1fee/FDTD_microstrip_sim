#include "../src/Simulation/PhysicalPlayback.h"
#include "../src/Simulation/Yee3D.h"
#include <limits>
#include <cstdio>
static void require(bool value) { if(!value) throw std::runtime_error("Physical playback regression"); }
template<class F> void rejects(F f) { bool rejected=false; try { f(); } catch(const std::invalid_argument&) { rejected=true; } require(rejected); }
int main() {
    const double dt=.5*25.4e-6/Yee3D::vacuumSpeed;
    PhysicalPlayback p;
    require(std::abs(p.stepsPerSecond(dt)-23605.70535433071)<1e-6);
    require(std::abs(dt*1e12-.042362640090165)<1e-12);
    p.referenceHz=2e9;
    require(std::abs(p.stepsPerSecond(dt)*dt-0.5e-9)<1e-24);
    p.cyclesPerSecond=.25;
    require(std::abs(p.simulatedSecondsPerSecond()-.125e-9)<1e-24);
    p.mode=PhysicalPlayback::PhysicalTime; p.picosecondsPerSecond=1000;
    require(std::abs(p.simulatedSecondsPerSecond()-1e-9)<1e-24);
    p.referenceHz=8e9; // reference changes cycle readout, not physical-time playback
    require(std::abs(p.stepsPerSecond(dt)*dt-1e-9)<1e-24);
    require(p.stepsPerSecond(dt/2)==2*p.stepsPerSecond(dt));
    rejects([&]{p.stepsPerSecond(0);});
    for(double bad : {0.0,-1.0,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()}) {
        auto invalid=p; invalid.referenceHz=bad; rejects([&]{invalid.stepsPerSecond(dt);});
        invalid=p; invalid.picosecondsPerSecond=bad; rejects([&]{invalid.stepsPerSecond(dt);});
        invalid=p; invalid.cyclesPerSecond=bad; rejects([&]{invalid.stepsPerSecond(dt);});
    }
    std::puts("RF/physical-time conversions, frequency changes, timestep refinement and invalid inputs passed.");
}
