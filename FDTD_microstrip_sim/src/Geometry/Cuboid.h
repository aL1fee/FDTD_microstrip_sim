#pragma once

#include "Geometry.h"

class Cuboid : public Geometry
{
protected:


public:
	Cuboid(glm::vec3 o, float l, float w, float h);

	virtual std::vector<glm::vec3>* build() override;



};