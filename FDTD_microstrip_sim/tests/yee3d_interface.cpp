#include "../src/Simulation/Yee3D.h"
#include <algorithm>
#include <cstdio>
#include <cmath>

int main()
{
    Yee3D field({320, 360, 2}, 1e-3);
    field.setPermittivity([](const auto& p) { return p[0] >= 0.160 ? 4.0 : 1.0; });
    for (std::size_t z = 0; z < 2; ++z)
        for (std::size_t y = 1; y < 360; ++y)
            for (std::size_t x = 1; x < 320; ++x)
            {
                const double u = (static_cast<double>(x) - 80) / 6;
                field.injectElectric(Yee3D::Z, {x, y, z}, static_cast<float>(std::exp(-0.5*u*u)));
            }

    double incident = 0, reflected = 0, transmitted = 0;
    for (int step = 1; step <= 320; ++step)
    {
        field.step();
        const double left = field.sampleElectric(Yee3D::Z, {120, 180, 1});
        const double right = field.sampleElectric(Yee3D::Z, {180, 180, 1});
        if (step < 140) incident = std::max(incident, left);
        if (step > 180) reflected = std::min(reflected, left);
        transmitted = std::max(transmitted, right);
    }
    const double r = reflected/incident, t = transmitted/incident;
    std::printf("Interface eps_r=4: r=%f (expected -1/3), t=%f (expected 2/3)\n",r,t);
    return std::abs(r+1.0/3) < 0.02 && std::abs(t-2.0/3) < 0.02 ? 0 : 1;
}
