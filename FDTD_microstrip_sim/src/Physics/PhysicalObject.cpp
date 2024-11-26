#include "PhysicalObject.h"

extern unsigned int physicalObjectNextId;

PhysicalObject::PhysicalObject()
{
	objectType = UNDEFINED;
	vertices = new VertexVectorDS();
	VAOs = new VAOVectorDS();
	origin = __initial_origin;
	length = __initial_zero_value;
	width = __initial_zero_value;
	height = __initial_zero_value;
	initial_length = length;
	initial_height = height;
	initial_width = width;
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	permittivity = __initial_zero_value;
	conductivity = __initial_zero_value;
	rebuiltExpected = false;
	builtExpected = true;
	isInteractable = true;
	isDeletable = true;
	edgesOn = true;
	shader = nullptr;
	propertyMap = new std::vector<std::pair<std::string, float*>>();
	propertyMapInt = new std::vector<std::pair<std::string, int*>>();
	id = physicalObjectNextId++;
	translationVector = __initial_translation_vec3;
	scalingVector = __initial_scaling_vec3;
	rotationVector = __initial_rotating_vec3;
	rotationAxis = __y_norm_vec3;
	rotationAngle = __initial_rotation_angle;
	modelMatrix = __initial_model_mat4;
	inverseModelMatrix = __initial_inverse_model_mat4;
	beingRendered = true;
	instancePositionsChanged = false;
	modelVertsRegenExpected = false;
	modelMatrixRegenExpected = false;
}

PhysicalObject::PhysicalObject(glm::vec3 o, float l, float w, float h, glm::vec3 col, float perm, float cond, Shader* sh)
{
	objectType = UNDEFINED;
	vertices = new VertexVectorDS();
	VAOs = new VAOVectorDS();
	origin = o;
	length = l;
	width = w;
	height = h;
	initial_length = length;
	initial_height = height;
	initial_width = width;
	color = col;
	permittivity = perm;
	conductivity = cond;
	rebuiltExpected = false;
	builtExpected = true;
	isInteractable = true;
	isDeletable = true;
	edgesOn = true;
	shader = sh;
	propertyMap = new std::vector<std::pair<std::string, float*>>();
	propertyMapInt = new std::vector<std::pair<std::string, int*>>();
	id = physicalObjectNextId++;
	translationVector = __initial_translation_vec3;
	scalingVector = __initial_scaling_vec3;
	rotationVector = __initial_rotating_vec3;
	rotationAxis = __y_norm_vec3;
	rotationAngle = __initial_rotation_angle;
	modelMatrix = __initial_model_mat4;
	inverseModelMatrix = __initial_inverse_model_mat4;
	beingRendered = true;
	instancePositionsChanged = false;
	modelVertsRegenExpected = false;
	modelMatrixRegenExpected = false;
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

void PhysicalObject::updateModelMatrix()
{
	glUniformMatrix4fv(shader->getUniformLocation("model"), 1, GL_FALSE,
		glm::value_ptr(modelMatrix));
}

glm::vec3 PhysicalObject::getCenterLocation() const
{
	//first version
	//glm::vec3 ret = glm::vec3(origin.x + length / 2.0f,
	//	origin.y + height / 2.0f, origin.z + width / 2.0f);
	//return ret;

	//second version
	//glm::vec3 localCenter = glm::vec3(length / 2.0f, height / 2.0f, 
	//	width / 2.0f);
	//glm::mat4 rotationMatrix = glm::rotate(__initial_model_mat4, 
	//	glm::radians(rotationAngle), rotationAxis);
	//glm::vec4 rotatedCenter = rotationMatrix * 
	//	glm::vec4(localCenter, 1.0f);
	//localCenter = glm::vec3(rotatedCenter);
	//glm::vec3 worldCenter = localCenter + origin;
	//return worldCenter;

	glm::vec3 localCenter = glm::vec3((length / 2.0f) / scalingVector.x, 
		(height / 2.0f) / scalingVector.y,
		(width / 2.0f) / scalingVector.z);
	return glm::vec3(modelMatrix * glm::vec4(localCenter, 1.0f));

	//glm::vec3 localCenter = glm::vec3(length / 2.0f, height / 2.0f, 
	//	width / 2.0f);
	//glm::mat4 rotationMatrix = glm::rotate(__initial_model_mat4, 
	//	glm::radians(rotationAngle), rotationAxis);
	//glm::vec4 rotatedCenter = rotationMatrix * 
	//	glm::vec4(localCenter, 1.0f);
	//localCenter = glm::vec3(rotatedCenter);
	//glm::vec3 worldCenter = localCenter + origin;
	//return worldCenter;


}

void PhysicalObject::setScale(float l, float h, float w)
{
	length = l;
	width = w;
	height = h;
}

glm::vec3 PhysicalObject::getLocalFromWorldPos(glm::vec3 v)
{
	return inverseModelMatrix * glm::vec4(v, 1.0f);
}

