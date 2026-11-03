#pragma once

#include "CavityController.h"
#include "../Scene/MainScene.h"

// Borrow scene objects only during reset. The controller retains coefficients.
inline void resetSceneCavity(CavityController& controller, MainScene& scene,
                            CavityController::Settings settings)
{
    if (settings.material != CavityController::Material::Scene)
    {
        controller.reset(settings);
        return;
    }
    const auto substrates = scene.getSubstrates();
    for (auto* substrate : substrates)
        substrate->generateModelMatrix();

    const auto conductors = scene.getConductors();
    for (auto* object : conductors) object->generateModelMatrix();

    controller.reset(settings, [&](const std::array<double, 3>& position) {
        const glm::vec3 mm(position[0]*1e3, position[1]*1e3, position[2]*1e3);
        float epsilon = 1;
        for (auto* substrate : substrates)
        {
            const float value = substrate->getRelativePermittivity();
            if (!std::isfinite(value) || value < 1)
                throw std::invalid_argument("Substrate relative permittivity must be finite and at least 1.");
            if (substrate->intersectionCheck(mm))
                epsilon = value;
        }
        return epsilon;
    }, [&](const std::array<double, 3>& position) {
        const glm::vec3 mm(position[0]*1e3, position[1]*1e3, position[2]*1e3);
        for (auto* object : conductors)
            if (object->intersectionCheck(mm)) return true;
        return false;
    });
}
