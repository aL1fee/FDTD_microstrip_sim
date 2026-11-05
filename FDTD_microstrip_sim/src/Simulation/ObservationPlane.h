#pragma once
#include "GridSettings.h"
#include "FieldSlice.h"

// Display-only snapshot of the active solver slice. Never participates in stepping.
struct ObservationPlane
{
    GridSettings grid;
    FieldSlice::Plane orientation = FieldSlice::XY;
    int layer = 0;
    bool visible = false;
    float opacity = .2f;
    bool domainVisible = false;
    Yee3D::Axis component = Yee3D::Z;
    bool colorByField = false;
    std::array<std::array<double,3>,4> cornersMeters() const
    {
        FieldSlice slice(grid.cells, orientation, layer, component);
        std::array<std::array<double,3>,4> corners{};
        for(int k=0;k<4;++k)
        {
            corners[k][slice.normal] = slice.position(grid.spacing);
            corners[k][slice.horizontal] = (k==1 || k==2) ? grid.cells[slice.horizontal]*grid.spacing : 0;
            corners[k][slice.vertical] = k>=2 ? grid.cells[slice.vertical]*grid.spacing : 0;
        }
        return corners;
    }
};
