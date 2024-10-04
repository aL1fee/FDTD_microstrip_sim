#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Macros.h"
#include "Renderer.h"
#include "GUI/GUI.h"

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

	glViewport(0, 0, FULL_SCREEN_SIZE_X, FULL_SCREEN_SIZE_Y);


	GUI* gui = new GUI(window);
	Renderer* renderer = new Renderer(window, gui);

	renderer->run();




	glfwDestroyWindow(window);
	glfwTerminate();
	exit(0);
}

