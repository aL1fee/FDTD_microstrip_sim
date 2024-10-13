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


class PhysicalObject
{
protected:

	glm::vec3 color;
	float permittivity;

public:
	PhysicalObject(glm::vec3 c, float p);






};