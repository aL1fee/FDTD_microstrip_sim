#include "PhysicalObject.h"

PhysicalObject::PhysicalObject()
{
	color = glm::vec3(0);
	permittivity = 0.0f;
	conductivity = 0.0f;
	isActivelyDrawn = true;
}

PhysicalObject::PhysicalObject(glm::vec3 c, float p)
{
	color = c;
	permittivity = p;
	conductivity = 0.0f;
	isActivelyDrawn = true;
}
