#pragma once

#include <vector>
#include <cmath>
#include <limits>

#include <iostream>

#include "glm.hpp"

#include <gtx/string_cast.hpp>

#include "../PhysicalObject.h"
#include "../../Macros.h"


class Cylinder_POT : public PhysicalObject
{
protected:
	float radius;
	glm::vec3 direction;

	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

public:
	Cylinder_POT(glm::vec3 o, glm::vec3 dir, float r, float l,
		glm::vec3 col, float perm, float cond, Shader* sh) :
		PhysicalObject(o, l, 0.0f, 0.0f, col, perm, cond, sh) 
	{
		radius = r;
		direction = dir;
		objectType = CYLINDER;
		width = radius;
		height = radius;

	}

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

	glm::vec3 getCenterLocation() const override;

	//void setScale(float l, float h, float w) override;
	void setScaleH(float h) override;
	void setScaleW(float w) override;

	float* getRadius() { return &radius; }


};