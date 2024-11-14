#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <gtx/string_cast.hpp>

#include "../PhysicalObject.h"

class Ribbon_POT : public PhysicalObject
{
protected:
	//length = abs length
	//width = abs width
	//height = height of ribbon
	
	float initialY;
	float finalY;
	float tail_size;
	float curve_height;

	bool terminated;
	bool firstPointSel;
	bool secondPointSel;
	bool beingDrawn;

	glm::vec3 firstPoint;
	glm::vec3 secondPoint;

	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;

	VertexVectorDS* feetVertices;
	VAOVectorDS* feetVAOs;
	VertexVectorDS* curveVertices;
	VAOVectorDS* curveVAOs;
	VertexVectorDS* edgeVertices;
	VAOVectorDS* edgeVAOs;


	void buildVertices() override;
	void rebuildVertices();
	void buildVAOs() override;
	void buildEdges() override;

	void rebuild() override;

	void generateRibbonVertices();
	float gaussian(float x, float A, float mu, float sigma);
	void buildFeet();

	void updateMaxXYZValues();

public:
	Ribbon_POT(glm::vec3 o, float l, float w, float h, glm::vec3 col,
		float perm, float cond, Shader* sh) :
		PhysicalObject(o, l, w, h, col, perm, cond, sh)
	{
		objectType = RIBBON;
		edgesOn = true;
		initialY = RIBBON_DEFAULT_INITIAL_Y;
		finalY = RIBBON_DEFAULT_FINAL_Y;
		tail_size = RIBBON_DEFAULT_TAIL_SIZE;
		curve_height = RIBBON_DEFAULT_CURVE_HEIGHT;
		width = RIBBON_INITIAL_WIDTH;
		height = RIBBON_INITIAL_HEIGHT;
		terminated = false;
		firstPointSel = false;
		secondPointSel = false;
		firstPoint = glm::vec3(0.0f);
		secondPoint = glm::vec3(0.0f);
		beingDrawn = true;
		feetVertices = new VertexVectorDS();
		feetVAOs = new VAOVectorDS();
		curveVertices = new VertexVectorDS();
		curveVAOs = new VAOVectorDS();
		edgeVertices = new VertexVectorDS();
		edgeVAOs = new VAOVectorDS();
		edgesOn = true;
		maxX = std::numeric_limits<float>::min();
		minX = std::numeric_limits<float>::max();
		maxY = std::numeric_limits<float>::min();
		minY = std::numeric_limits<float>::max();
		maxZ = std::numeric_limits<float>::min();
		minZ = std::numeric_limits<float>::max();
	}


	void build() override;
	void draw() override;
	bool intersectionCheck(glm::vec3 v) override;
	void clear() override {}

	bool isTerminated() const { return terminated; }
	bool firstPointSelected() const { return firstPointSel; }
	void setSecondPointSelected(bool b) { secondPointSel = b; }

	void setFirstPoint(glm::vec3 v);
	void setSecondPotentialPoint(glm::vec3 v);

	void terminate() { terminated = true; }

	void setBeingDrawn(bool b) { beingDrawn = false; }

	glm::vec3 getFirstPoint() const { return firstPoint; }

};