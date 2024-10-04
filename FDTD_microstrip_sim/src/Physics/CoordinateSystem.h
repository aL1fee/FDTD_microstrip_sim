#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

class CoordinateSystem
{
private:
	float helperLineSeparation;
	float length;

	//std::vector<unsigned int>* mainVAOs;
	unsigned int mainVAO;
	unsigned int helperVAO;

	std::array<glm::vec3, 6>* mainVerts;
	std::vector<glm::vec3>* helperVerts;

	glm::vec3 mainColor;
	glm::vec3 helperColor;

public:
	CoordinateSystem(float l, float mls);

	void buildMainVertices();
	void buildMainVAO();
	void drawMain() const;

	void buildHelperVertices();
	void buildHelperVAO();
	void drawHelper() const;

	glm::vec3 getMainColor() const { return mainColor; }
	glm::vec3 getHelperColor() const { return helperColor; }

	void run();

};