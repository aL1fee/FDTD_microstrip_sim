#pragma once

#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <stack>

#include <windows.h>

#include <thread>
#include <mutex>


#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "GUI/GUI.h"
#include "Setup/Shader.h"
#include "Utility.h"
#include "Macros and variables/Macros.h"

#include "Input/Input.h"

//#include "Geometry/CoordinateSystem.h"
#include "Scene/Camera.h"
#include "Scene/MainScene.h"

#include "Simulation/SimulationSpace.h"


void window_size_callback(GLFWwindow* window, int width, int height);

void end_resize(GLFWwindow* window);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Renderer
{
private:
	GLFWwindow* window;
	Input* input;
	GUI* gui;

	void updateDeltaTime() const;


	void testPerformanceStart() const;
	void testPerformanceStop() const;

	void centeredViewportResize() const;

	void enablePolygonOffset() const;
	void disablePolygonOffset() const;
	void disableLineSmoothing() const;
	void setLineWidth(float value) const;
	void enableDepthTest() const;
	void disableDepthTest() const;

public:
	Renderer(GLFWwindow* w, GUI* g);

	void clearColorBuffer() const;
	void clearDepthBuffer() const;

	void updateFrameBackgroundColor() const;

	void run() const;

	void render() const;
	//void updateSimulation() const;







};