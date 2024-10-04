#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include "Macros.h"

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);
//void window_refresh_callback(GLFWwindow* window);


enum backgroundColor {
	GREY,
	LIGHT,
	DARK
};

class Utility
{
private:


public:

	static void checkGLError(const char* function);






};