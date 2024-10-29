#pragma once

#include "../PO Templates (POTs)/Cuboid_POT.h"

#include <vector>
#include <string>

class Trace_PO : public Cuboid_POT
{
private:
	float charImpledance;

public:
	Trace_PO(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh) :
		Cuboid_POT(o, l, w, h, col, perm, cond, sh) 
	{
		charImpledance = 50.0f;
	}

	void updatePropertyMap() override;
};