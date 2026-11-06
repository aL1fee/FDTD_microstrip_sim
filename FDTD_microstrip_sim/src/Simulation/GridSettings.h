#pragma once
#include "Yee3D.h"

// Physical grid settings shared by the scene adapter and headless controller.
struct GridSettings
{
    Yee3D::Index cells{24,20,16};
    double spacing = 0.0005;
    static GridSettings fromDomain(std::array<double,3> meters, double dx)
    {
        if (!std::isfinite(dx) || dx <= 0) throw std::invalid_argument("Cell size must be finite and positive.");
        GridSettings grid;
        grid.spacing = dx;
        for (int axis=0; axis<3; ++axis)
        {
            double count = meters[axis] / dx;
            if (!std::isfinite(count) || count < 2-1e-6 || count > 1000+1e-3)
                throw std::invalid_argument("3D grid needs 2 to 1000 cells per axis. Adjust domain dimensions or cell size.");
            // Scene inputs are floats: recover intended integer ratios before flooring.
            if (std::abs(count-std::round(count)) < 1e-6 * std::max(1.0, count)) count = std::round(count);
            grid.cells[axis] = static_cast<std::size_t>(std::floor(count));
        }
        return grid;
    }
    std::size_t cellCount() const { return cells[0]*cells[1]*cells[2]; }
    std::size_t electricCount() const
    {
        return cells[0]*(cells[1]+1)*(cells[2]+1) + (cells[0]+1)*cells[1]*(cells[2]+1) + (cells[0]+1)*(cells[1]+1)*cells[2];
    }
    std::size_t residentBytes() const
    {
        const auto h = (cells[0]+1)*cells[1]*cells[2] + cells[0]*(cells[1]+1)*cells[2] + cells[0]*cells[1]*(cells[2]+1);
        return sizeof(float)*(2*electricCount()+h) + (electricCount()+7)/8 + 4096;
    }
    std::size_t resetBytes() const { return residentBytes() + sizeof(float)*electricCount() + (electricCount()+7)/8; }
    static std::size_t memoryBudget() { return 512ull*1024*1024; }
    void validate(std::size_t previousBytes = 0) const
    {
        if (!std::isfinite(spacing) || spacing <= 0) throw std::invalid_argument("Cell size must be finite and positive.");
        for (auto n : cells) if (n<2 || n>1000) throw std::invalid_argument("3D grid needs 2 to 1000 cells per axis.");
        if (resetBytes() + std::max(previousBytes, residentBytes()) > memoryBudget())
            throw std::invalid_argument("Grid exceeds the internal memory limit. Increase cell size or reduce the domain.");
    }
    bool operator==(const GridSettings& other) const { return cells==other.cells && spacing==other.spacing; }
    bool operator!=(const GridSettings& other) const { return !(*this==other); }
};
