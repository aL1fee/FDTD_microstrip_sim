#pragma once

#include <iostream>
//#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

//#include "../GUI/GUI.h"
//#include "../Input/Input.h"

#include "../Scene/Camera.h"

#include "../Macros and variables/Macros.h"

#include "../Setup/Shader.h"

extern Camera* cam;

extern bool _projMatrixChanged;

class Scene
{
protected:
	GLFWwindow* window;
	//GUI* gui;
	//Input* input;
	float aspect;


public:
	//Scene(GLFWwindow* w, GUI* g, Input* in);
	Scene();
	Scene(GLFWwindow* w);


	virtual void init() = 0;
	virtual void render() = 0;
};