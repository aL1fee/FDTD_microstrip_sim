#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <stdio.h>
#include <math.h>

#include "glm.hpp"
#include <gtx/string_cast.hpp>


#include "../Geometry/GeometricalObject.h"
#include "../Setup/Shader.h"

//axes:
//	x: red
//  z: blue
//  y: green

enum PhysicalObjectType {
	UNDEFINED,
	CUBOID,
	TRACE,
	CYLINDER
};

class PhysicalObject : public GeometricalObject
{
protected:
	PhysicalObjectType objectType;
	float permittivity;
	float conductivity;
	bool rebuiltExpected;
	bool isInteractable;
	Shader* shader;
	std::vector<std::pair<std::string, float*>>* propertyMap;
	unsigned int id;

public:
	PhysicalObject();
	PhysicalObject(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh);
	~PhysicalObject();

	virtual void updatePropertyMap() {};
	std::vector<std::pair<std::string, float*>>* getPropertyMap() { return propertyMap; }

	bool needsRebuilding() { return rebuiltExpected; }
	bool interactable() const { return isInteractable; }
	void setRebuiltExpected(bool b) { rebuiltExpected = b; }

	std::string getShaderName() const { return shader->getName(); }
	unsigned int getId() const { return id; }

	glm::vec3 getCenterLocation() const;
	glm::vec3* getOrigin() { return &origin; }

	void setOrigin(glm::vec3 v) { origin = v; }
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

};