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


class Geometry 
{
protected:
	glm::vec3 origin;
	//float length; //x
	//float width; //z
	//float height; //y

	std::vector<glm::vec3>* vertices;
	std::vector<unsigned int>* VAOs;

public:
	virtual std::vector<glm::vec3>* build() = 0;



};