#pragma once

#include "PhysicalObject.h"
#include "../Geometry/Cuboid.h"

class Carrier_PO : public PhysicalObject, public Cuboid
{
private:


public:
	Carrier_PO(glm::vec3 o, float l, float w, float h, glm::vec3 c, float p) : Cuboid(o, l, w, h), PhysicalObject(c, p) {}



};