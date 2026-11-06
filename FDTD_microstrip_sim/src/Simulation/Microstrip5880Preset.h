#pragma once
#include "SimulationSpace.h"

// Scene coordinates are mm; this example is specified in mil (0.0254 mm).
// Insertion follows the other Add examples: existing objects are retained.
namespace Microstrip5880Preset
{
    constexpr float mil = .0254f;
    inline void add(MainScene& scene, SimulationSpace& simulation)
    {
        constexpr float epsilon0 = 8.8541878e-12f;
        std::string name = "Microstrip on Duroid 5880";
        scene.addCarrier(name, glm::vec3(0), 200*mil, 100*mil, 5*mil,
            glm::vec3(1,1,.66f), epsilon0, 4.1e7f);
        scene.addSubstrate(name, glm::vec3(0,5*mil,20*mil), 200*mil,60*mil,5*mil,
            glm::vec3(.4f,.8f,1), 2.2f*epsilon0, 0);
        scene.getSubstrates().back()->setRelativePermittivity(2.2f);
        scene.getSubstrates().back()->setReferenceLossTangent(.0009f,10);
        scene.addTrace(name, glm::vec3(0,10*mil,43*mil), 200*mil,14*mil,1.4f*mil,
            glm::vec3(1,.843f,0), epsilon0, 5.8e7f);
        // Tangent to the trace top: center Y = trace base + thickness + radius.
        // The existing volume drive now excites air above the trace, not metal.
        const float sourceY = 10*mil + 1.4f*mil + 5*mil;
        scene.addPowerSource(name, glm::vec3(2*mil,sourceY,50*mil), glm::vec3(1,0,0),
            -10, 5*mil, 3*mil, glm::vec3(1,.843f,0), epsilon0, 0);
        auto* source = scene.getPowerSources().back();
        *source->getFrequency() = 1; // GHz, as in the reference example
        *source->getWavelength() = static_cast<float>(Yee3D::vacuumSpeed/1e9*1000);
        *source->getPolarization3D() = Yee3D::Y;
        *source->getDriveAmplitude3D() = 1;
        simulation.setDimensions(glm::vec3(200*mil,30*mil,100*mil));
        *simulation.getCellSize() = 1*mil;
        simulation.setSimulationDimension(3);
        simulation.setCellUpdate(true);
        simulation.initilizeFields();
    }
}
