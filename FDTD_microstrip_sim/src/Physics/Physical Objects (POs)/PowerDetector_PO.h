#pragma once

#include "../PO Templates (POTs)/Cylinder_POT.h"

#include <vector>
#include <string>

class PowerDetector_PO : public Cylinder_POT
{
private:
	float sensitivity;

public:
	PowerDetector_PO(glm::vec3 o, glm::vec3 dir, float s, float r, float l,
		glm::vec3 col, float perm, float cond, Shader* sh) :
		Cylinder_POT(o, dir, r, l, col, perm, cond, sh)
	{
		sensitivity = s;
	}

	void updatePropertyMap() override;
};