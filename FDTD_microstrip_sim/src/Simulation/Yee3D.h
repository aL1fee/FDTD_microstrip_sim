#pragma once

#include <array>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

// Cubic Yee cells with lossless isotropic dielectrics and PEC walls. Magnetic samples store Z0 * H.
class Yee3D
{
public:
    using Index = std::array<std::size_t, 3>;
    enum Axis { X, Y, Z };
    static constexpr double vacuumSpeed = 299792458.0;

    Yee3D(Index cells, double spacingMeters, float courant = 0.5f)
        : coefficient(courant), spacing(spacingMeters), dt(courant * spacingMeters / vacuumSpeed)
    {
        if (!std::isfinite(dt) || dt <= 0 || !(courant > 0 && courant < 1.0 / std::sqrt(3.0)))
            throw std::invalid_argument("Invalid spacing or 3D Courant number");

        for (auto count : cells)
            if (count < 2 || count == std::numeric_limits<std::size_t>::max())
                throw std::invalid_argument("Invalid cavity cell count");

        for (int axis = X; axis <= Z; ++axis)
        {
            Index eSize = cells;
            Index hSize = cells;
            ++hSize[axis];
            for (int other = X; other <= Z; ++other)
                if (other != axis)
                    ++eSize[other];
            electric[axis] = Grid(eSize);
            electricCoefficient[axis] = Grid(eSize);
            conductor[axis].resize(electric[axis].samples.size(), false);
            for (auto& value : electricCoefficient[axis].samples) value = courant;
            magnetic[axis] = Grid(hSize);
        }
    }

    // Material callback takes each staggered E location in meters.
    // Build separately so an invalid material leaves the current grid intact.
    template<class Material>
    void setPermittivity(Material material)
    {
        auto next = electricCoefficient;
        for (int axis = X; axis <= Z; ++axis)
        {
            auto& grid = next[axis];
            for (std::size_t z = 0; z < grid.size[Z]; ++z)
                for (std::size_t y = 0; y < grid.size[Y]; ++y)
                    for (std::size_t x = 0; x < grid.size[X]; ++x)
                    {
                        std::array<double, 3> position{x * spacing, y * spacing, z * spacing};
                        position[axis] += 0.5 * spacing;
                        const double epsilon = material(position);
                        if (!std::isfinite(epsilon) || epsilon < 1.0 ||
                            epsilon > std::numeric_limits<float>::max())
                            throw std::invalid_argument("Relative permittivity must be finite and at least 1");
                        grid.at({x, y, z}) = coefficient / static_cast<float>(epsilon);
                    }
        }
        electricCoefficient.swap(next);
    }

    using ConductorSampler = std::function<bool(const std::array<double, 3>&)>;
    void setConductors(const ConductorSampler& contains)
    {
        auto next = conductor;
        for (int axis = X; axis <= Z; ++axis)
        {
            auto& grid = electric[axis];
            for (std::size_t z = 0; z < grid.size[Z]; ++z)
                for (std::size_t y = 0; y < grid.size[Y]; ++y)
                    for (std::size_t x = 0; x < grid.size[X]; ++x)
                    {
                        std::array<double, 3> point{x*spacing, y*spacing, z*spacing};
                        point[axis] += 0.5*spacing;
                        next[axis][grid.offset({x,y,z})] = contains(point);
                    }
        }
        conductor.swap(next);
        for (int axis = X; axis <= Z; ++axis)
            for (std::size_t i = 0; i < conductor[axis].size(); ++i)
                if (conductor[axis][i]) electric[axis].samples[i] = 0;
    }

    std::size_t conductorSamples() const
    {
        std::size_t total = 0;
        for (const auto& mask : conductor)
            total += std::count(mask.begin(), mask.end(), true);
        return total;
    }

    double timestep() const { return dt; }

    float sampleElectric(Axis axis, Index index) const
    {
        return electric[axis].checked(index);
    }

    float sampleMagnetic(Axis axis, Index index) const
    {
        return magnetic[axis].checked(index);
    }

    void injectElectric(Axis axis, Index index, float amplitude)
    {
        auto& grid = electric[axis];
        float& sample = grid.checked(index);
        for (int other = X; other <= Z; ++other)
            if (other != axis && (index[other] == 0 || index[other] == grid.size[other] - 1))
                throw std::invalid_argument("Electric source on a PEC wall");
        if (!conductor[axis][grid.offset(index)])
            sample += amplitude;
    }

    void step()
    {
        for (int axis = X; axis <= Z; ++axis)
        {
            const int b = (axis + 1) % 3;
            const int c = (axis + 2) % 3;
            auto& h = magnetic[axis];
            const auto& eb = electric[b];
            const auto& ec = electric[c];
            for (std::size_t z = 0; z < h.size[Z]; ++z)
                for (std::size_t y = 0; y < h.size[Y]; ++y)
                    for (std::size_t x = 0; x < h.size[X]; ++x)
                    {
                        const Index p{x, y, z};
                        Index nextB = p, nextC = p;
                        ++nextB[b];
                        ++nextC[c];
                        h.at(p) -= coefficient * ((ec.at(nextB) - ec.at(p)) -
                                                  (eb.at(nextC) - eb.at(p)));
                    }
        }

        for (int axis = X; axis <= Z; ++axis)
        {
            const int b = (axis + 1) % 3;
            const int c = (axis + 2) % 3;
            auto& e = electric[axis];
            const auto& hb = magnetic[b];
            const auto& hc = magnetic[c];
            // Skip the tangential electric edges on the six PEC walls.
            Index begin{1, 1, 1};
            begin[axis] = 0;
            Index end = e.size;
            --end[b];
            --end[c];
            for (std::size_t z = begin[Z]; z < end[Z]; ++z)
                for (std::size_t y = begin[Y]; y < end[Y]; ++y)
                    for (std::size_t x = begin[X]; x < end[X]; ++x)
                    {
                        const Index p{x, y, z};
                        Index prevB = p, prevC = p;
                        --prevB[b];
                        --prevC[c];
                        if (conductor[axis][e.offset(p)]) continue;
                        e.at(p) += electricCoefficient[axis].at(p) * ((hc.at(p) - hc.at(prevB)) -
                                                  (hb.at(p) - hb.at(prevC)));
                    }
        }
    }

private:
    struct Grid
    {
        Index size{};
        std::vector<float> samples;

        Grid() = default;
        explicit Grid(Index extent) : size(extent)
        {
            const auto max = samples.max_size();
            if (size[X] > max / size[Y] || size[X] * size[Y] > max / size[Z])
                throw std::length_error("Yee grid exceeds addressable storage");
            samples.resize(size[X] * size[Y] * size[Z]);
        }

        std::size_t offset(Index p) const
        {
            return (p[Z] * size[Y] + p[Y]) * size[X] + p[X];
        }

        float& at(Index p) { return samples[offset(p)]; }
        float at(Index p) const { return samples[offset(p)]; }

        const float& checked(Index p) const
        {
            for (int axis = X; axis <= Z; ++axis)
                if (p[axis] >= size[axis])
                    throw std::out_of_range("Yee sample outside grid");
            return samples[offset(p)];
        }
        float& checked(Index p)
        {
            static_cast<const Grid&>(*this).checked(p);
            return at(p);
        }
    };

    float coefficient;
    double spacing;
    double dt;
    std::array<Grid, 3> electricCoefficient;
    std::array<std::vector<bool>, 3> conductor;
    std::array<Grid, 3> electric;
    std::array<Grid, 3> magnetic;
};
