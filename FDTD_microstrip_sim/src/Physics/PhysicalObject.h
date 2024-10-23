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

#include "../Geometry/Geometry.h"


class PhysicalObject
{
protected:
	glm::vec3 color;
	float permittivity;
	bool isActivelyDrawn;

public:
	PhysicalObject();
	PhysicalObject(glm::vec3 c, float p);

	glm::vec3 getColor() const { return color; }


};