#include "PhysicalObject.h"

extern unsigned int physicalObjectNextId;

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
	propertyMap = new std::vector<std::pair<std::string, float*>>();
	id = physicalObjectNextId++;
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
	id = physicalObjectNextId++;
}

PhysicalObject::~PhysicalObject()
{
	delete vertices;
	delete VAOs;
	delete propertyMap;
	if (shader->getNumObjectsServed() == 0) {
		std::cout << "a shader is deleted!" << std::endl;
		delete shader;
	}
}

glm::vec3 PhysicalObject::getCenterLocation() const
{
	glm::vec3 ret = glm::vec3(origin.x + length / 2.0f,
		origin.y + height / 2.0f, origin.z + width / 2.0f);
	return ret;
}


