#pragma once
#include "Yee3D.h"
#include <string>

// Reset-time geometry is borrowed; only mapped indices and scalar settings survive reset.
struct ElectricDriveDefinition
{
    unsigned int id = 0;
    Yee3D::Axis axis = Yee3D::Y;
    double frequencyHz = 2e9;
    double amplitude = 1;
    std::function<bool(const std::array<double,3>&)> contains;
};
struct ElectricDrive
{
    unsigned int id;
    Yee3D::Axis axis;
    double frequencyHz, amplitude;
    std::vector<Yee3D::Index> samples;
    std::size_t excluded = 0;
    double rate(double t) const
    {
        const double pi=std::acos(-1.0);
        const double ramp=.5*(1-std::cos(pi*std::min(1.0,t*frequencyHz/2)));
        return amplitude*2*pi*frequencyHz*ramp*std::sin(2*pi*frequencyHz*t);
    }
};
