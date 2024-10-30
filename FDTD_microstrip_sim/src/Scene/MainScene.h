#pragma once

#include <iostream>
#include <map>
#include <string>

#include "Scene.h"
#include "../Physics/Physical Objects (POs)/CoordinateSystem_PO.h"
#include "../Physics/Physical Objects (POs)/Carrier_PO.h"
#include "../Physics/Physical Objects (POs)/Substrate_PO.h"
#include "../Physics/Physical Objects (POs)/Trace_PO.h"
#include "../Physics/Physical Objects (POs)/Housing_PO.h"
#include "../Physics/Physical Objects (POs)/Curve_PO.h"

#include "../Data Structures/VertexVectorDS.h"
#include "../Data Structures/VAOVectorDS.h"

#include "../GUI/PropertyWindow.h"

#include "../Physics/Physical Objects (POs)/ModifyingVectors_PO.h"

class MainScene : public Scene
{
private:
	std::map<std::string, Shader*>* shaderMap;
	std::map<unsigned int, PhysicalObject*>* physicalObjectBuffer;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	PropertyWindow* propertyWindow;

	Curve_PO* testingLine;
	Shader* testingLineShader;

	PhysicalObject* activeObject;

	float nearPlaneValue;
	float farPlaneValue;

	//VertexVectorDS* modifyingVectorsVertices;
	//VAOVectorDS* modifyingVectorsVAOs;

	ModifyingVectors_PO* modifyingVectors;

	void eraseShaderMapOneInstance(std::string name);

	//void buildModifyingVectors();
	//void clearModifyingVectors();

public:
	MainScene(GLFWwindow* w);

	void init() override;
	void render() override;

	void addCarrier(std::string& s, glm::vec3 o, float l, float w, 
		float h, glm::vec3 col, float perm, float cond);
	void addSubstrate(std::string& s, glm::vec3 o, float l, float w,
		float h, glm::vec3 col, float perm, float cond);
	void addTrace(std::string& s, glm::vec3 o, float l, float w,
		float h, glm::vec3 col, float perm, float cond);
	void addHousing(std::string& s, glm::vec3 o, float l, float w,
		float h, glm::vec3 col, float perm, float cond);

	void deleteActiveObject();
	void deleteAllObjects();

	void setProjMatrix(glm::mat4 m) { projMatrix = m; }

	glm::mat4 getProjMatrix() const { return projMatrix; }
	glm::mat4 getViewMatrix() const { return viewMatrix; }

	Curve_PO* getTestingLine() { return testingLine; }

	void generateRay(glm::vec3 pos, glm::vec3 dir);
	void selectObject(glm::vec3 pos, glm::vec3 dir);

	PropertyWindow* getPropertyWindow() { return propertyWindow; }

	PhysicalObject* getActiveObject() const { return activeObject; }

	float* getNearPlaneValue() { return &nearPlaneValue; }
	float* getFarPlaneValue() { return &farPlaneValue; }

	void updateProjMatrix() { _projMatrixChanged = true; }

	void buildModifyingVectors(PhysicalObject* obj, ModyfingVectorType type);
	void deleteModifyingVectors();
	ModifyingVectors_PO* getModifyingVectors() { return modifyingVectors; }

	void preObjectInsertionSetup();
	void postObjectInsertionSetup();

};