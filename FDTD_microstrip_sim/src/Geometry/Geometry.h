#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdio.h>
#include <math.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>

#include "../Data Structures/VAOVectorDS.h"
#include "../Data Structures/VertexVectorDS.h"

class Geometry 
{
protected:
	glm::vec3 origin;
	//float length; //x
	//float width; //z
	//float height; //y

	VertexVectorDS* vertices;
	VAOVectorDS* VAOs;

public:
	virtual void build() = 0;



};