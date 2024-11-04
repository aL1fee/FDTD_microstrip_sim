#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class InputBuffer
{
private:
	std::unordered_map<int, bool> keyStateMap;
	GLFWwindow* window;
	void initializeKeys();

public:
	InputBuffer(GLFWwindow* w);

	void processKeyStates();
	bool checkKeyState(int key);
	void setKeyState(int key, bool val);
};