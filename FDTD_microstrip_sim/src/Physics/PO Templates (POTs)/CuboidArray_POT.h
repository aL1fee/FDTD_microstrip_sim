#pragma once

#include "../PhysicalObject.h"

class CuboidArray_POT : public PhysicalObject
{
protected:
	int numPadsX;
	int numPadsZ;
	float sizeX;
	float sizeZ;
	float padSeparationX;
	float padSeparationZ;

	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

public:
	CuboidArray_POT(glm::vec3 o, int npX, int npZ,
		float padSepX, float padSepZ, float sX, float sZ, glm::vec3 col,
		float perm, float cond,
		Shader* sh) :
		PhysicalObject(o, 0.0f, 0.0f, 0.01f, col, perm, cond, sh)
	{
		sizeX = sX;
		sizeZ = sZ;
		numPadsX = npX;
		numPadsZ = npZ;
		padSeparationX = padSepX;
		padSeparationZ = padSepZ;
		length = numPadsX * sizeX + (numPadsX - 1) * padSeparationX;
		width = numPadsZ * sizeZ + (numPadsZ - 1) * padSeparationZ;
		//objectType = CUBOID;
	}

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

	int* getNumPadsX() { return &numPadsX; }
	int* getNumPadsZ() { return &numPadsZ; }
	float* getSizeX() { return &sizeX; }
	float* getSizeZ() { return &sizeZ; }
	float* getPadSeparationX() { return &padSeparationX; }
	float* getPadSeparationZ() { return &padSeparationZ; }

	glm::vec3 getCenterLocation() const override;
	void setScaleL(float l) override;
	void setScaleW(float w) override;
};