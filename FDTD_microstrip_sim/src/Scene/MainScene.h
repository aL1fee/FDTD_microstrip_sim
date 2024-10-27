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

#include "../GUI/PropertyWindow.h"

class MainScene : public Scene
{
private:
	std::map<std::string, Shader*>* shaderMap;
	std::vector<PhysicalObject*>* physicalObjectBuffer;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
	PropertyWindow* propertyWindow;

	Curve_PO* testingLine;
	Shader* testingLineShader;

	PhysicalObject* activeObject;

public:
	MainScene(GLFWwindow* w);

	void init() override;
	void render() override;

	void addCarrier(std::string& s);
	void addSubstrate(std::string& s);
	void addTrace(std::string& s);
	void addHousing(std::string& s);

	void deleteActiveObject();
	void deleteAllObjects();

	void setProjMatrix(glm::mat4 m) { projMatrix = m; }

	glm::mat4 getProjMatrix() const { return projMatrix; }
	glm::mat4 getViewMatrix() const { return viewMatrix; }

	Curve_PO* getTestingLine() { return testingLine; }

	void generateRay(glm::vec3 pos, glm::vec3 dir);
	void selectObject(glm::vec3 pos, glm::vec3 dir);

	PropertyWindow* getPropertyWindow() { return propertyWindow; }
};