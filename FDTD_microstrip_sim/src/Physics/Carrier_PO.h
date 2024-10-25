#pragma once

#include "PhysicalObject.h"
#include "../Geometry/Cuboid.h"

#include <vector>
#include <string>

class Carrier_PO : public PhysicalObject, public Cuboid
{
private:
	std::vector<std::pair<std::string, float*>>* propertyMap;

	void init();



public:
	Carrier_PO();
	Carrier_PO(glm::vec3 o, float l, float w, float h, glm::vec3 c, float p) : Cuboid(o, l, w, h, c), PhysicalObject(c, p) {
		propertyMap = new std::vector<std::pair<std::string, float*>>();
		init();
		build();
	}

	void updatePropertyMap();
	std::vector<std::pair<std::string, float*>>* getPropertyMap();

};