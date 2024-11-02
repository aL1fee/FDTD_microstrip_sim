#pragma once

#include "../PhysicalObject.h"

class Cuboid_POT : public PhysicalObject
{
protected:
	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

public:
	Cuboid_POT(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh) :
		PhysicalObject(o, l, w, h, col, perm, cond, sh) 
	{
		objectType = CUBOID;
	}

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

};