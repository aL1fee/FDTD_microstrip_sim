#include "../src/Simulation/Yee3D.h"
#include <cmath>
#include <stdexcept>

int main()
{
    Yee3D vacuum({4, 4, 4}, 0.001);
    Yee3D dielectric({4, 4, 4}, 0.001);
    int samples = 0;
    dielectric.setPermittivity([&](const auto& p) {
        int halfCoordinates = 0;
        for (double coordinate : p)
        {
            const double halfUnits = coordinate / 0.0005;
            if (std::abs(halfUnits - std::round(halfUnits)) > 1e-8)
                throw std::runtime_error("Incorrect material sample position");
            halfCoordinates += static_cast<int>(std::lround(halfUnits)) % 2;
        }
        if (halfCoordinates != 1)
            throw std::runtime_error("Material sampled away from an electric edge");
        ++samples;
        return 4.0;
    });
    if (samples != 300) return 1;

    try {
        dielectric.setPermittivity([](const auto& p) { return p[0] > 0.002 ? 0.0 : 2.0; });
        return 2;
    } catch (const std::invalid_argument&) {}

    vacuum.injectElectric(Yee3D::Z, {2,2,2}, 1);
    dielectric.injectElectric(Yee3D::Z, {2,2,2}, 1);
    vacuum.step();
    dielectric.step();
    // Failed material update must preserve the original uniform epsilon=4.
    const float air = vacuum.sampleElectric(Yee3D::Z, {1,2,2});
    const float medium = dielectric.sampleElectric(Yee3D::Z, {1,2,2});
    return air != 0 && std::abs(medium * 4 - air) < 1e-6 ? 0 : 3;
}
