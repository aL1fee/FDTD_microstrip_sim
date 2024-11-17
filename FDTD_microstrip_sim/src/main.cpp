#include <Windows.h>
#undef APIENTRY

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Macros and variables/Macros.h"
#include "Renderer.h"
#include "GUI/GUI.h"

Renderer* renderer;

static bool glfwInitialize() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit()) {
		return false;
	}
	return true;
}

static GLFWwindow* createWindow() 
{
	GLFWwindow* window;
	window = glfwCreateWindow(INITIAL_GLFW_WINDOW_WIDTH, INITIAL_GLFW_WINDOW_HEIGHT, "FDTD Microstrip simulation", NULL, NULL);
	glfwSetWindowPos(window, INITIAL_GLFW_WINDOW_POSITION_X, INITIAL_GLFW_WINDOW_POSITION_Y);
	glfwMaximizeWindow(window);
	if (!window) {
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	return window;
}

static bool loadGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

int main() {

	if (!glfwInitialize())
	{
		return -1;
	}
	GLFWwindow* window = createWindow();
	if (!loadGlad()) {
		return -1;
	}


	extern int _initialWindowWidth;
	extern int _initialWindowHeight;
	extern int _currentWindowWidth;
	extern int _currentWindowHeight;
	//int width, height;
	glfwGetWindowSize(window, &_initialWindowWidth, &_initialWindowHeight);
	_currentWindowWidth = _initialWindowWidth;
	_currentWindowHeight = _initialWindowHeight;
	glViewport(0, 0, _initialWindowWidth, _initialWindowHeight);
	extern float _aspectRatio;

	_aspectRatio = (float) _initialWindowWidth / _initialWindowHeight;
	//std::cout << "AR: " << _aspectRatio << std::endl;
	//std::cout << "width: " << _initialWindowWidth << std::endl;
	//std::cout << "height: " << _initialWindowHeight << std::endl;


	//glViewport(0, 0, FULL_SCREEN_SIZE_X, FULL_SCREEN_SIZE_Y);
	 
	
	//glView port(INITIAL_OPENGL_CONTEXT_POSITION_X, INITIAL_OPENGL_CONTEXT_POSITION_Y,
	//	INITIAL_OPENGL_CONTEXT_SCREEN_WIDTH, INITIAL_OPENGL_CONTEXT_SCREEN_HEIGHT);


	GUI* gui = new GUI(window);
	renderer = new Renderer(window, gui);

	renderer->run();




	glfwDestroyWindow(window);
	glfwTerminate();
	exit(0);
}

