#include "../src/Simulation/Yee3D.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

int main()
{
    for (int direction = 0; direction < 3; ++direction)
    {
        const int transverse = (direction + 1) % 3;
        const auto polarization = static_cast<Yee3D::Axis>((direction + 2) % 3);
        Yee3D::Index extent{};
        extent[direction] = 320;
        extent[transverse] = 360;
        extent[polarization] = 2;
        Yee3D field(extent, 1e-3);
        field.setConductors([direction](const auto& p) {
            return p[direction] >= 0.160 && p[direction] <= 0.170;
        });
        if (field.conductorSamples() == 0)
            return 1;

        Yee3D::Index p{};
        for (p[polarization] = 0; p[polarization] < 2; ++p[polarization])
            for (p[transverse] = 1; p[transverse] < 360; ++p[transverse])
                for (p[direction] = 1; p[direction] < 320; ++p[direction])
                {
                    const double u = (static_cast<double>(p[direction]) - 80) / 6;
                    field.injectElectric(polarization, p, static_cast<float>(std::exp(-0.5*u*u)));
                }

        double incident = 0, reflected = 0, transmitted = 0;
        p[polarization] = 1;
        p[transverse] = 180;
        for (int step = 1; step <= 320; ++step)
        {
            field.step();
            p[direction] = 120;
            const double left = field.sampleElectric(polarization, p);
            p[direction] = 180;
            const double right = field.sampleElectric(polarization, p);
            if (!std::isfinite(left) || !std::isfinite(right)) return 2;
            if (step < 140) incident = std::max(incident, left);
            if (step > 180) reflected = std::min(reflected, left);
            transmitted = std::max(transmitted, std::abs(right));
            p[direction] = 165;
            field.injectElectric(polarization, p, 1.0f);
            if (field.sampleElectric(polarization, p) != 0) return 3;
        }
        if (incident < 0.1) return 4;
        const double reflection = reflected / incident;
        std::printf("PEC normal %c: reflection %.6f, transmission %.6g\n",
                    "xyz"[direction], reflection, transmitted / incident);
        if (std::abs(reflection + 1) > 0.02 || transmitted / incident > 1e-6)
            return 5;
    }

    // Empty mask restores the ability to inject into formerly blocked samples.
    Yee3D field({4,4,4}, 1e-3);
    field.setConductors([](const auto&) { return true; });
    field.setConductors([](const auto&) { return false; });
    field.injectElectric(Yee3D::Z, {2,2,2}, 1);
    if (field.conductorSamples() != 0 || field.sampleElectric(Yee3D::Z,{2,2,2}) != 1)
        return 6;
}
