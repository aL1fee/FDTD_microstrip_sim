#pragma once
#include <cmath>
#include <stdexcept>

// Playback changes the wall-clock schedule, never the numerical timestep.
struct PhysicalPlayback
{
    enum Mode { RFCycles, PhysicalTime };
    Mode mode=RFCycles;
    double cyclesPerSecond=1;
    double picosecondsPerSecond=1000;
    double referenceHz=1e9;
    double simulatedSecondsPerSecond() const {
        if(!std::isfinite(referenceHz) || referenceHz<=0 ||
           !std::isfinite(cyclesPerSecond) || cyclesPerSecond<=0 ||
           !std::isfinite(picosecondsPerSecond) || picosecondsPerSecond<=0 ||
           (mode!=RFCycles && mode!=PhysicalTime))
            throw std::invalid_argument("Playback values must be finite and positive.");
        const double result=mode==RFCycles ? cyclesPerSecond/referenceHz : picosecondsPerSecond*1e-12;
        if(!std::isfinite(result) || result<=0) throw std::invalid_argument("Invalid physical playback rate.");
        return result;
    }
    double stepsPerSecond(double dt) const {
        if(!std::isfinite(dt) || dt<=0) throw std::invalid_argument("Invalid physical timestep.");
        const double result=simulatedSecondsPerSecond()/dt;
        if(!std::isfinite(result) || result<=0) throw std::invalid_argument("Invalid playback step rate.");
        return result;
    }
};
