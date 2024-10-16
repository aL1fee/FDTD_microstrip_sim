#pragma once

#include <iostream>

#include "Scene.h"
#include "../Physics/CoordinateSystem.h"
#include "../Physics/Carrier_PO.h"
#include "../Physics/TestingLine.h"

class MainScene : public Scene
{
private:
	CoordinateSystem csys;
	Shader shader_csys;

	GLuint viewCamLoc;
	GLuint projCamLoc;
	GLuint colCsysLoc;

	glm::vec3 mainGridColor;
	glm::vec3 helperGridColor;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	std::vector<PhysicalObject*>* physicalObjectBuffer;

	TestingLine* testingLine;


public:
	//MainScene(GLFWwindow* w, GUI* g, Input* in) :Scene(w, g, in), shader_csys("res/shaders/coordinate_system.shader", 0), csys(0.0f, 0.0f), viewCamLoc(0), projCamLoc(0), colCsysLoc(0),
	//	mainGridColor(1.0f, 1.0f, 1.0f), helperGridColor(0.5f, 0.5f, 0.5f), viewMatrix(1.0f), projMatrix(1.0f) {
	//	physicalObjectBuffer = new std::vector<PhysicalObject>();
	//	init();
	//}
	//MainScene(GLFWwindow* w, Input* in) : Scene(w, in), shader_csys("res/shaders/coordinate_system.shader", 0), csys(0.0f, 0.0f), viewCamLoc(0), projCamLoc(0), colCsysLoc(0),
	//	mainGridColor(1.0f, 1.0f, 1.0f), helperGridColor(0.5f, 0.5f, 0.5f), viewMatrix(1.0f), projMatrix(1.0f) {
	//	physicalObjectBuffer = new std::vector<PhysicalObject*>();
	//	init();
	//}
	MainScene(GLFWwindow* w) : Scene(w), shader_csys("res/shaders/coordinate_system.shader", 0), csys(0.0f, 0.0f), viewCamLoc(0), projCamLoc(0), colCsysLoc(0),
		mainGridColor(1.0f, 1.0f, 1.0f), helperGridColor(0.5f, 0.5f, 0.5f), viewMatrix(1.0f), projMatrix(1.0f) {
		physicalObjectBuffer = new std::vector<PhysicalObject*>();
		init();
	}

	void init() override;
	void render() override;

	void addCarrier(std::string& s);

	void setProjMatrix(glm::mat4 m) { projMatrix = m; }

	glm::mat4 getProjMatrix() const { return projMatrix; }
	glm::mat4 getViewMatrix() const { return viewMatrix; }

	TestingLine* getTestingLine() { return testingLine; }

};