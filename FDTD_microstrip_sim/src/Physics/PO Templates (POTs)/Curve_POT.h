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

#include "../PhysicalObject.h"
#include "../../Geometry/GeometricalObject.h"
#include "../../Data Structures/VertexVectorDS.h"

class Curve_POT : public PhysicalObject
{
protected:
	int builtUntilIndex;

	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override {}


public:
	Curve_POT(Shader* sh);

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;

	void addPoint(glm::vec3 v);
	void terminateLine();
};