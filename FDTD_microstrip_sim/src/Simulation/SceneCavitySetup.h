#pragma once

#include "CavityController.h"
#include "../Scene/MainScene.h"

struct ConductorCoverage
{
    unsigned int id;
    glm::vec3 originMm;
    glm::vec3 sizeMm; // X length, Y height, Z width
    std::size_t samples = 0;
};
using SceneCoverage = std::vector<ConductorCoverage>;

// Borrow scene objects only during reset. The controller retains coefficients.
inline void resetSceneCavity(CavityController& controller, MainScene& scene,
                            CavityController::Settings settings, SceneCoverage* coverage = nullptr)
{
    std::vector<ElectricDriveDefinition> sources;
    if(settings.excitation == CavityController::Excitation::SceneSources)
        for(auto* source : scene.getPowerSources())
        {
            source->generateModelMatrix();
            sources.push_back({source->getId(), static_cast<Yee3D::Axis>(*source->getPolarization3D()),
                *source->getFrequency()*1e9, *source->getDriveAmplitude3D(),
                [source](const std::array<double,3>& p) {
                    if(!std::isfinite(*source->getRadius()) || *source->getRadius()<=0 ||
                       !std::isfinite(*source->getLength()) || *source->getLength()<=0)
                        throw std::invalid_argument("Source radius and length must be finite and positive.");
                    return source->intersectionCheck(glm::vec3(p[0]*1e3,p[1]*1e3,p[2]*1e3));
                }});
        }
    if (settings.material != CavityController::Material::Scene)
    {
        if (controller.reset(settings, {}, {}, sources) && coverage) coverage->clear();
        return;
    }
    const auto substrates = scene.getSubstrates();
    for (auto* substrate : substrates)
        substrate->generateModelMatrix();

    const auto conductors = scene.getConductors();
    for (auto* object : conductors) object->generateModelMatrix();

    SceneCoverage nextCoverage;
    for (auto* object : conductors)
        nextCoverage.push_back({object->getId(),
            {*object->getOriginX(), *object->getOriginY(), *object->getOriginZ()},
            {*object->getLength(), *object->getHeight(), *object->getWidth()}, 0});
    const bool accepted = controller.reset(settings, [&](const std::array<double, 3>& position) {
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
        bool inside = false;
        for (std::size_t i = 0; i < conductors.size(); ++i)
            if (conductors[i]->intersectionCheck(mm))
            {
                ++nextCoverage[i].samples;
                inside = true;
            }
        return inside;
    }, sources);
    if (accepted && coverage) *coverage = std::move(nextCoverage);
}
