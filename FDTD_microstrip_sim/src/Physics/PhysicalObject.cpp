#include "PhysicalObject.h"

PhysicalObject::PhysicalObject()
{
	vertices = new VertexVectorDS();
	VAOs = new VAOVectorDS();
	origin = glm::vec3(0.0f);
	length = 0.0f;
	width = 0.0f;
	height = 0.0f;
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	permittivity = 0.0f;
	conductivity = 0.0f;
	rebuiltExpected = true;
	isInteractable = true;
	edgesOn = true;
	shader = nullptr;
	// TODO: generate a default propertyMap
	propertyMap = new std::vector<std::pair<std::string, float*>>();;
}

PhysicalObject::PhysicalObject(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh)
{
	vertices = new VertexVectorDS();
	VAOs = new VAOVectorDS();
	origin = o;
	length = l;
	width = w;
	height = h;
	color = col;
	permittivity = perm;
	conductivity = cond;
	rebuiltExpected = true;
	isInteractable = true;
	edgesOn = true;
	shader = sh;
	propertyMap = new std::vector<std::pair<std::string, float*>>();
}

PhysicalObject::~PhysicalObject()
{
	delete vertices;
	delete VAOs;
	delete propertyMap;
	delete shader;
}

