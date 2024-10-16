#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>

#include "PhysicalObject.h"
#include "../Geometry/Geometry.h"

class TestingLine : public Geometry
{
protected:
	void buildVertices();
	void buildVAOs();

public:
	TestingLine();

	std::vector<glm::vec3>* build() override;
	void draw() const;
	void addPoint(glm::vec3 v);


};