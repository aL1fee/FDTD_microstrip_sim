#pragma once

#include "../PO Templates (POTs)/Ribbon_POT.h"

#include <vector>
#include <string>

class Ribbon_PO : public Ribbon_POT
{
private:


public:
	Ribbon_PO(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh) :
		Ribbon_POT(o, l, w, h, col, perm, cond, sh)
	{}

	void updatePropertyMap() override;
};