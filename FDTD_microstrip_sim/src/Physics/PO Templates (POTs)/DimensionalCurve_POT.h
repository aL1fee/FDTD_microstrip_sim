#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <gtx/string_cast.hpp>

#include "../PhysicalObject.h"

class DimensionalCurve_POT : public PhysicalObject
{
protected:
	int indexProcessed;
	glm::vec3 unprocessedVertex;
	std::vector<glm::vec3> processedVertices;
	bool movementStarted;

	float diameter;

	bool curveTerminated;
	bool lastVertexEntered;

	bool beingDrawn;

	glm::vec3 prevNormal1;
	glm::vec3 prevNormal2;
	glm::vec3 prevCenter;

	glm::vec3 lastVertex;

	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;

	int processedVertsSize;

	VertexVectorDS* cylindersVertices;
	VAOVectorDS* cylindersVAOs;

	VertexVectorDS* edgesVertices;
	VAOVectorDS* edgesVAOs;

	VertexVectorDS* crossSecVertices;
	VAOVectorDS* crossSecVAOs;


	VertexVectorDS* ballFootVertices;
	VAOVectorDS* ballFootVAOs;

	void buildVertices() override;
	void rebuildVertices();
	void buildVAOs() override;
	void buildEdges() override;

	void rebuild() override;

public:
	DimensionalCurve_POT(glm::vec3 o, float l, float w, float h, glm::vec3 col, 
		float perm, float cond, Shader* sh) :
		PhysicalObject(o, l, w, h, col, perm, cond, sh)
	{
		processedVertsSize = 0;
		diameter = DIMENSIONAL_CURVE_DIAMETER;
		maxX = std::numeric_limits<float>::min();
		minX = std::numeric_limits<float>::max();
		maxY = std::numeric_limits<float>::min();
		minY = std::numeric_limits<float>::max();
		maxZ = std::numeric_limits<float>::min();
		minZ = std::numeric_limits<float>::max();
		objectType = CURVE;
		indexProcessed = 0;
		unprocessedVertex = glm::vec3(0.0f);
		movementStarted = false;
		cylindersVertices = new VertexVectorDS();
		cylindersVAOs = new VAOVectorDS();
		crossSecVertices = new VertexVectorDS();
		crossSecVAOs = new VAOVectorDS();
		crossSecVertices->allocateNEmptyArrays(1);
		edgesVertices = new VertexVectorDS();
		edgesVertices->allocateNEmptyArrays(3);
		edgesVAOs = new VAOVectorDS();
		ballFootVertices = new VertexVectorDS();
		ballFootVertices->allocateNewEmptyArray();
		ballFootVertices->allocateNewEmptyArray();
		ballFootVAOs = new VAOVectorDS();
		prevNormal1 = glm::vec3(0.0f);
		prevNormal2 = glm::vec3(0.0f);
		prevCenter = glm::vec3(0.0f);
		edgesOn = false;
		curveTerminated = false;
		lastVertexEntered = false;
		beingDrawn = true;
		lastVertex = glm::vec3(0.0f);

	}

	void addPoint(glm::vec3 v);
	void updateUnprocessedVertex(glm::vec3 v);

	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

	glm::vec3 getMaxPoint() { return glm::vec3(maxX, maxY, maxZ); }

	void termiateTheCurve() { curveTerminated = true; }

	void setBeingDrawn(bool b) { beingDrawn = b; }

	void setLastVertex(glm::vec3 v) { lastVertex = v; }

	bool isTerminated() const { return curveTerminated; }

	bool isLastPointEntered() const { return lastVertexEntered; }

	void lastPointEntered(bool b) { lastVertexEntered = b; }

	glm::vec3 getCenterLocation() const override;
};