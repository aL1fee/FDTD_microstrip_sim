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

#include "../Data Structures/VAOVectorDS.h"
#include "../Data Structures/VertexVectorDS.h"

class Geometry 
{
protected:
	glm::vec3 origin;

	VertexVectorDS* vertices;
	VAOVectorDS* VAOs;

public:
	virtual void build() = 0;
	virtual void draw() = 0;
	virtual bool intersectionCheck(glm::vec3 v) = 0;


};