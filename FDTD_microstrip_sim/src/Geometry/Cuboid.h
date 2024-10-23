#pragma once

#include "Geometry.h"

class Cuboid : public Geometry
{
protected:
	float length; //x
	float width; //z
	float height; //y
	bool buildingEdges;

	glm::vec3 color;

	void buildEdges();

	void addColorVertex(glm::vec3 v);

public:
	Cuboid();
	Cuboid(glm::vec3 o, float l, float w, float h, glm::vec3 c);

	virtual void build() override;

	void buildVertices();
	void buildVAOs();

	void draw() override;




};