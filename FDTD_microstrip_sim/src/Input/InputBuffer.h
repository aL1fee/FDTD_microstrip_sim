#pragma once

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
//#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class InputBuffer
{
private:
	std::vector<glm::vec3>* leftClickBuffer;


public:
	InputBuffer();

	void addLeftClick(glm::vec3 v);
	void clearLeftClickBuffer();
	glm::vec3 getLeftClick(int i) const;




};