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

#include "PhysicalObject.h"
#include "../Geometry/Geometry.h"

#include "../Data Structures/VertexVectorDS.h"

class TestingLine : public Geometry
{
protected:
	void buildVertices();
	void buildVAOs();

	int builtUntilIndex;

public:
	TestingLine();

	void build() override;
	void draw() override;
	void addPoint(glm::vec3 v);
	void terminateLine();


};