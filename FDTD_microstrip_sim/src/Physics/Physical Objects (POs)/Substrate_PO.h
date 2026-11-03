#pragma once

#include "../PO Templates (POTs)/Cuboid_POT.h"

#include <vector>
#include <string>

class Substrate_PO : public Cuboid_POT
{
private:
	float relativePermittivity = 4.0f;


public:
	Substrate_PO(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh) :
		Cuboid_POT(o, l, w, h, col, perm, cond, sh) {}

	float getRelativePermittivity() const { return relativePermittivity; }
	void updatePropertyMap() override;
};