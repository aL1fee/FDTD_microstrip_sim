#pragma once

#include "../PO Templates (POTs)/CuboidArray_POT.h"

#include <vector>
#include <string>

class TuningPadArray : public CuboidArray_POT
{
private:


public:
	TuningPadArray(glm::vec3 o, int npX, int npZ,
		float padSepX, float padSepZ, float sX, float sZ, glm::vec3 col,
		float perm, float cond, Shader* sh) :
		CuboidArray_POT(o, npX, npZ, padSepX, padSepZ, sX, sZ,
			col, perm, cond, sh) {}

	void updatePropertyMap() override;
};