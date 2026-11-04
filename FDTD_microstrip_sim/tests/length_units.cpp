#include "../src/GUI/LengthUnits.h"
#include "../src/Simulation/CavityController.h"
#include <cstdio>
#include <stdexcept>

static void require(bool ok) { if (!ok) throw std::runtime_error("Length units / coverage regression"); }
int main()
{
    using namespace LengthUnits;
    for (double mm : {0.0, -1.25, 0.02, 2.0, 12000.0})
        require(std::abs(micrometersToScene(sceneToMicrometers(mm))-mm) < 1e-12);
    require(displayName("Height (mm)") == "Height (um)");
    require(propertyName("Height (mm)") == "Height");
    require(propertyName("Rotation angle (deg)") == "Rotation angle");
    require(!isSceneLength("Permittivity (F/m)"));
    require(metersToMicrometers(0.0005) == 500.0);

    for (auto unit : {Unit::Mil, Unit::Millimeter, Unit::Micrometer})
        for (double mm : {0.0, -1.25, 0.02, 2.0, 200.0})
            require(std::abs(toScene(fromScene(mm, unit), unit)-mm) < 1e-12);
    require(std::abs(fromScene(0.0254, Unit::Mil)-1) < 1e-12);
    require(displayName("Height (mm)", Unit::Mil) == "Height (mil)");
    require(displayName("Height (mm)", Unit::Millimeter) == "Height (mm)");
    CavityController cavity;
    auto block = [](const std::array<double, 3>& p) {
        // Same strict-interior convention as scene cuboids, expressed in UI units.
        const double x=metersToMicrometers(p[0]), y=metersToMicrometers(p[1]), z=metersToMicrometers(p[2]);
        return x>5000 && x<7000 && y>1000 && y<3000 && z>1000 && z<3000;
    };
    require(cavity.reset({CavityController::Material::Scene, 1},
        [](const auto&) { return 1.0; }, block));
    require(cavity.conductorSamples() > 0);
    require(cavity.sample({12,4,4}) == 0);
    std::printf("2000 um block: %zu PEC electric samples\n", cavity.conductorSamples());
    for (int n=0; n<100; ++n) cavity.stepOnce();
    require(cavity.sample({12,4,4}) == 0);
    // A sub-grid slab between staggered X locations cannot be represented.
    require(cavity.reset({CavityController::Material::Scene, 1},
        [](const auto&) { return 1.0; }, [](const auto& p) {
            return p[0] > 0.0001 && p[0] < 0.00011;
        }));
    require(cavity.conductorSamples() == 0);
    std::puts("Length conversion and resolved/unresolved conductor checks passed.");
}
