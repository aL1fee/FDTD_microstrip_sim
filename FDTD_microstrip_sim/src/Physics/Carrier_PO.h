#pragma once

#include "PhysicalObject.h"
#include "../Geometry/Cuboid.h"

class Carrier_PO : public PhysicalObject, public Cuboid
{
private:
	void init();

public:
	Carrier_PO();
	Carrier_PO(glm::vec3 o, float l, float w, float h, glm::vec3 c, float p) : Cuboid(o, l, w, h, c), PhysicalObject(c, p) {
		//init();
		build();
	}



};