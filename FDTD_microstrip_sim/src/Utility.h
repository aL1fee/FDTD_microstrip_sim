#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);

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