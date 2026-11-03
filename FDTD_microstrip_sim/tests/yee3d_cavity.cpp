#include "../src/Simulation/Yee3D.h"

#include <cmath>
#include <cstdio>
#include <stdexcept>

// Excite the mode with E along the chosen axis and one half-wave along
// each transverse axis. Average positive-going zero crossings at a probe.
double measure(Yee3D::Axis axis, Yee3D::Index cells, double spacing, float epsilon = 1.0f)
{
    Yee3D cavity(cells, spacing);
    cavity.setPermittivity([epsilon](const auto&) { return epsilon; });
    const double pi = std::acos(-1.0);
    const int b = (axis + 1) % 3;
    const int c = (axis + 2) % 3;
    const double expected = Yee3D::vacuumSpeed / (2 * spacing * std::sqrt(epsilon)) *
        std::hypot(1.0 / cells[b], 1.0 / cells[c]);

    Yee3D::Index p{};
    for (p[axis] = 0; p[axis] < cells[axis]; ++p[axis])
        for (p[b] = 1; p[b] < cells[b]; ++p[b])
            for (p[c] = 1; p[c] < cells[c]; ++p[c])
                cavity.injectElectric(axis, p,
                    static_cast<float>(std::sin(pi * p[b] / cells[b]) *
                                       std::sin(pi * p[c] / cells[c])));

    const Yee3D::Index probe{cells[0] / 2, cells[1] / 2, cells[2] / 2};
    double previous = cavity.sampleElectric(axis, probe);
    double first = 0, last = 0;
    int crossings = 0;
    const int steps = static_cast<int>(8 / (expected * cavity.timestep()));
    for (int n = 1; n <= steps; ++n)
    {
        cavity.step();
        const double value = cavity.sampleElectric(axis, probe);
        if (!std::isfinite(value) || std::abs(value) > 1.01)
            throw std::runtime_error("Unstable cavity field");
        if (previous < 0 && value >= 0)
        {
            last = (n - 1 - previous / (value - previous)) * cavity.timestep();
            if (crossings++ == 0)
                first = last;
        }
        previous = value;
    }
    if (crossings < 5)
        throw std::runtime_error("Too few resonance cycles");

    const double measured = (crossings - 1) / (last - first);
    const double error = std::abs(measured / expected - 1);
    std::printf("E%c, dx %.2f mm: analytical %.6f GHz, measured %.6f GHz, error %.4f%%\n",
                "xyz"[axis], spacing * 1e3, expected * 1e-9, measured * 1e-9, error * 100);
    return error;
}

int main()
{
    for (float epsilon : {1.0f, 4.0f})
    for (const auto axis : {Yee3D::X, Yee3D::Y, Yee3D::Z})
    {
        const double coarse = measure(axis, {12, 10, 8}, 1e-3, epsilon);
        const double fine = measure(axis, {24, 20, 16}, 0.5e-3, epsilon);
        if (coarse >= 0.01 || fine >= coarse * 0.35)
            return 1;
    }
}
