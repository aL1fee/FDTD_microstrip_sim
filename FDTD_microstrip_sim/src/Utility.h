#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include "Macros.h"

#include "Scene/MainScene.h"

//#include "Scene/Scene.h"
// //#include "Renderer.h"

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);
//void window_refresh_callback(GLFWwindow* window);

enum backgroundColor {
	GREY,
	LIGHT,
	DARK
};

struct positions {
	float x;
	float y;
	float z;
};

class Utility
{
private:


public:

	static void checkGLError(const char* function);

	static glm::vec3 screenToNDC(GLFWwindow* window, float x, float y, float z);

	static glm::vec4 NDCToHCC(glm::vec3 v);

	static glm::vec4 clipToEyeC(glm::vec4 v);

	static glm::vec3 eyeToWorldC(glm::vec4 v);

	bool add_overflow(int x, int y, int* result);

};