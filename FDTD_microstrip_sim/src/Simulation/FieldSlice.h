#pragma once
#include "Yee3D.h"

// Native component samples have a half-cell offset along their polarization axis.
struct FieldSlice
{
    enum Plane { XY, XZ, YZ };
    int horizontal, vertical, normal;
    Yee3D::Index samples;
    std::size_t layer;
    Yee3D::Axis component;
    FieldSlice(Yee3D::Index cells, Plane plane, int requested, Yee3D::Axis axis=Yee3D::Z)
        : samples(cells), component(axis)
    {
        for(int a=0;a<3;++a) if(a!=component) ++samples[a];
        // Keep scene height (Y) vertical in the transverse cross-section.
        horizontal = plane == YZ ? 2 : 0;
        vertical = plane == XZ ? 2 : 1;
        normal = plane == XY ? 2 : plane == XZ ? 1 : 0;
        layer = static_cast<std::size_t>(std::max(0, std::min(requested, static_cast<int>(samples[normal])-1)));
    }
    std::size_t width() const { return samples[horizontal]; }
    std::size_t height() const { return samples[vertical]; }
    Yee3D::Index index(std::size_t u, std::size_t v) const
    {
        if(u>=width() || v>=height()) throw std::out_of_range("Slice sample outside grid");
        Yee3D::Index p{}; p[horizontal]=u; p[vertical]=v; p[normal]=layer; return p;
    }
    double position(double dx) const { return (layer+(normal==component ? .5 : 0))*dx; }
};
