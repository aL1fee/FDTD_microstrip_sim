#pragma once

#include "../PhysicalObject.h"

class DimensionalCurve_POT : public PhysicalObject
{
protected:
	int indexProcessed;
	std::vector<glm::vec3> unprocessedVertices;
	std::vector<glm::vec3> processedVertices;


	void buildVertices() override;
	void buildVAOs() override;
	void buildEdges() override;

	void rebuild() override;

	


public:
	DimensionalCurve_POT(glm::vec3 o, float l, float w, float h, glm::vec3 col, 
		float perm, float cond, Shader* sh) :
		PhysicalObject(o, l, w, h, col, perm, cond, sh)
	{
		objectType = UNDEFINED;
		indexProcessed = 0;
	}

	void addPoint(glm::vec3 v);


	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

};