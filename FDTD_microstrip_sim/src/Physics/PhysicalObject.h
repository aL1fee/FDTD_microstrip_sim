#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdio.h>
#include <math.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "../Macros and variables/Macros.h"
#include "../Macros and variables/Global variables.h"

#include "../Geometry/GeometricalObject.h"
#include "../Setup/Shader.h"

//axes:
//	x: red
//  z: blue
//  y: green

extern bool __centerOrientedRotation;

enum PhysicalObjectType {
	UNDEFINED,
	CUBOID,
	TRACE,
	CYLINDER,
	CURVE,
	RIBBON
};

class PhysicalObject : public GeometricalObject
{
protected:
	PhysicalObjectType objectType;
	float permittivity;
	float conductivity;
	bool rebuiltExpected;
	bool builtExpected;
	bool isInteractable;
	bool isDeletable;
	Shader* shader;
	std::vector<std::pair<std::string, float*>>* propertyMap;
	std::vector<std::pair<std::string, int*>>* propertyMapInt;
	unsigned int id;

	bool beingRendered;

	virtual void updateModelMatrix();

public:
	PhysicalObject();
	PhysicalObject(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh);
	~PhysicalObject();

	virtual void updatePropertyMap() {};
	std::vector<std::pair<std::string, float*>>* getPropertyMap() { return propertyMap; }
	std::vector<std::pair<std::string, int*>>* getPropertyMapInt() { return propertyMapInt; }

	bool needsRebuilding() { return rebuiltExpected; }
	bool needsBuilding() { return builtExpected; }
	bool interactable() const { return isInteractable; }
	bool deletable() const { return isDeletable; }
	void setRebuiltExpected(bool b) { rebuiltExpected = b; }
	void setBuiltExpected(bool b) { builtExpected = b; }

	std::string getShaderName() const { return shader->getName(); }
	unsigned int getId() const { return id; }

	virtual glm::vec3 getCenterLocation() const;
	glm::vec3* getOrigin() { return &origin; }

	void setRotationAxis(glm::vec3 v) { rotationAxis = v; }
	void setOrigin(glm::vec3 v) { origin = v; }
	void setRotationAngle(float angle) { rotationAngle = angle; }

	virtual void setScale(float l, float h, float w);
	virtual void setScaleL(float l) { length = l; }
	virtual void setScaleH(float h) { height = h; }
	virtual void setScaleW(float w) { width = w; }

	void setInteractable(bool val) { isInteractable = val; }

	bool operator==(const PhysicalObject& other) const {
		return id == other.getId();
	}

	void setId(unsigned int ui) { id = ui; }

	PhysicalObjectType getObjectType() const { return objectType; }

	void setBeingRendered(bool b) { beingRendered = b; }

	virtual void generateModelMatrix() {};

};