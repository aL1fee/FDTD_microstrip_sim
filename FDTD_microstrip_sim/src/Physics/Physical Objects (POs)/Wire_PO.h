#pragma once

#include "../PO Templates (POTs)/DimensionalCurve_POT.h"

#include <vector>
#include <string>

class Wire_PO : public DimensionalCurve_POT
{
private:


public:
	Wire_PO(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh) :
		DimensionalCurve_POT(o, l, w, h, col, perm, cond, sh)
	{}

	void updatePropertyMap() override;
};