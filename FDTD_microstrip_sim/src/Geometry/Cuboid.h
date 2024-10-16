#pragma once

#include "Geometry.h"

class Cuboid : public Geometry
{
protected:
	float length; //x
	float width; //z
	float height; //y

public:
	Cuboid();
	Cuboid(glm::vec3 o, float l, float w, float h);

	virtual std::vector<glm::vec3>* build() override;



};