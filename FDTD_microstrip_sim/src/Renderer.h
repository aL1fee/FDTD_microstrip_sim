#pragma once

#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <stack>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "GUI/GUI.h"
#include "Setup/Shader.h"
#include "Utility.h"
#include "Macros.h"

#include "Input.h"

#include "Physics/CoordinateSystem.h"
#include "Scene/Camera.h"

class Renderer
{
private:
	GLFWwindow* window;
	Input* input;
	GUI* gui;

	void updateDeltaTime() const;



public:
	Renderer(GLFWwindow* w, GUI* g);

	void clearColorBuffer() const;
	void clearDepthBuffer() const;

	void updateFrameBackgroundColor() const;

	void run() const;









};