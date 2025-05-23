#pragma once

#include <iostream>
#include <vector>

//#include <GLFW/glfw3.h>
//#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../Macros and Variables/Macros.h"
#include "../Macros and Variables/Global variables.h"

#include "../Scene/MainScene.h"
#include "../Scene/Camera.h"
#include "InputBuffer.h"

#include "../Utility.h"

//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
//void mouse_callback(GLFWwindow* window, int button, int action, int mods);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

class Input
{
private:
	GLFWwindow* window;
	std::vector<int>* potentialPressedGLFWButtons;
	InputBuffer* inputBuffer;




public:
	Input(GLFWwindow* w);

	void processInput();

	static void updateModifyingVectors(GLFWwindow* window, double xpos, double ypos);



};
