#pragma once

#include <iostream>
#include <string>
#include <map>

#include "../Simulation/CavityController.h"
#include "../Simulation/PropagationValidation.h"
#include "../Simulation/ObservationPlane.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

#include "StatusWindow.h"
#include "PropertyWindow.h"

#include "../Scene/MainScene.h"
#include "../Simulation/SimulationSpace.h"

extern bool _wireZYplane;
extern bool _wireXYplane;
extern bool _wireInputExpected;

class GUI
{
private:
	GLFWwindow* window;
	StatusWindow* statusWindow;
	
	float firstRightPanelMinimized;
	float propertyWindowMinimized;

	void buildMainMenuPanel();
	void buildTheoryWindow();
	void buildCavityWindow();
    CavityController::Settings requested3DSettings() const;
    bool canRun3D() const;

	int cavityMaterialMode = 0;
    int cavityGridSource = 0; // 0: configured domain, 1: explicit validation preset
    int activeGridSource = -1;
    bool solverBoundaryVisible = true;
	float cavityPermittivity = 4.0f;
	CavityController cavity;
    PropagationValidation propagationValidation;
	bool cavityOpen = false;
	int cavitySlice = 8;
    int cavityPlane = 0;
    int cavityExcitation = 0;
    int cavityComponent = 2;
    bool observationPlaneVisible = true;
    bool observationPlaneField = true;
    float observationPlaneOpacity = .2f;
	double cavityLastFrame = 0;

	bool theoryOpen = false;
	int theoryTopic = -1;
	std::string theoryText;
	void buildMenuUpperPanel();
	void buildMenuLowerPanel();
	void buildLeftPanel();
	void buildLowerPanel();
	void buildRightPanels();

	ImFont* customFont;



public:

	GUI(GLFWwindow* w);

	void init();
	void render();
	void terminate();

    const CavityController& fieldSimulation() const { return cavity; }
    ObservationPlane observationPlane() const
    {
        return {cavity.grid(), static_cast<FieldSlice::Plane>(cavityPlane), cavitySlice,
            cavity.ready() && observationPlaneVisible, observationPlaneOpacity, cavity.ready() && solverBoundaryVisible, static_cast<Yee3D::Axis>(cavityComponent), observationPlaneField};
    }
    void newFrame();





};