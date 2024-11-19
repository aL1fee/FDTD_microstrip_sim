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
#include "../Physics/Physical Objects (POs)/PowerSource_PO.h"
#include "../Physics/Physical Objects (POs)/PowerDetector_PO.h"
#include "../Physics/Physical Objects (POs)/TuningPadArray_PO.h"
#include "../Physics/Physical Objects (POs)/PowerDetector_PO.h"
#include "../Physics/Physical Objects (POs)/Wire_PO.h"

#include "../Physics/Physical Objects (POs)/Ribbon_PO.h"

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

	DimensionalCurve_POT* activeWire;
	Ribbon_POT* activeRibbon;

	glm::vec3 highestClickedObjPoint;

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
	void addPowerSource(std::string& s, glm::vec3 o, glm::vec3 dir, float p,
		float r, float l, glm::vec3 col, float perm, float cond);
	void addPowerDetector(std::string& s, glm::vec3 o, glm::vec3 dir, float sens,
		float r, float l, glm::vec3 col, float perm, float cond);
	void addTuningPadArray(std::string& s, glm::vec3 o, int npX, int npZ,
		float padSepX, float padSepZ, float sX, float sZ, glm::vec3 col,
		float perm, float cond);
	void addWire(std::string& s, glm::vec3 o, float l, float w, float h, 
		glm::vec3 col, float perm, float cond);
	void addRibbon(std::string& s, glm::vec3 o, float l, float w, float h,
		glm::vec3 col, float perm, float cond);


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

	void showModifyingVectors(PhysicalObject* obj, ModyfingVectorType type);
	void deleteModifyingVectors();
	void hideModifyingVectors() { modifyingVectors->setBeingRendered(false); }
	ModifyingVectors_PO* getModifyingVectors() { return modifyingVectors; }

	void preObjectInsertionSetup();
	void postObjectInsertionSetup();

	void clearLines();

	DimensionalCurve_POT* getActiveWire();
	Ribbon_POT* getActiveRibbon();

	glm::vec3 getHighestClickedObjPoint();

	void updateHighestClickedObjPoint(glm::vec3 pos, glm::vec3 dir);
	PhysicalObject* higherPointObject(glm::vec3 v, glm::vec3 maxV);

};