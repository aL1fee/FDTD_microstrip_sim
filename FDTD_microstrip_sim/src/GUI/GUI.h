#pragma once

#include <iostream>
#include <string>
#include <map>

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

	void newFrame();





};