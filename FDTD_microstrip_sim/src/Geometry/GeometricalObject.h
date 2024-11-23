#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>

#include "../Data Structures/VAOVectorDS.h"
#include "../Data Structures/VertexVectorDS.h"

class GeometricalObject
{
protected:
	glm::vec3 origin;
	float length;
	float width;
	float height;
	float initial_length;
	float initial_height;
	float initial_width;
	glm::vec3 color;
	VertexVectorDS* vertices;
	VAOVectorDS* VAOs;
	bool edgesOn;

	glm::vec3 translationVector;
	glm::vec3 scalingVector;
	glm::vec3 rotationVector;
	float rotationAngle;
	glm::vec3 rotationAxis;
	glm::mat4 modelMatrix;
	glm::mat4 inverseModelMatrix;

	//bool rotatable;
	//bool translatable;
	//bool scalable;

	bool instancePositionsChanged;

	void addColorVertex(glm::vec3 v) { vertices->pushToExistingArray(v); }

	virtual void buildVertices() = 0;
	virtual void buildVAOs() = 0;
	virtual void buildEdges() = 0;
	virtual void clear() = 0;

	virtual void rebuild() = 0;


public:
	virtual void build() = 0;
	virtual void draw() = 0;
	virtual bool intersectionCheck(glm::vec3 v) = 0;

	float* getLength() { return &length; }
	float* getWidth() { return &width; }
	float* getHeight() { return &height; }

	float* getOriginX() { return &origin.x; };
	float* getOriginY() { return &origin.y; };
	float* getOriginZ() { return &origin.z; };

	float* getRotationAngle() { return &rotationAngle; }

	glm::vec3* getColor() { return &color; }

};