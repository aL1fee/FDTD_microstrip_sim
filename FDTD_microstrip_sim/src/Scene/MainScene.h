#pragma once

#include "Scene.h"
#include "../Physics/CoordinateSystem.h"

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

public:
	MainScene(GLFWwindow* w, GUI* g, Input* in) :Scene(w, g, in), shader_csys("res/shaders/coordinate_system.shader", 0), csys(0.0f, 0.0f), viewCamLoc(0), projCamLoc(0), colCsysLoc(0),
		mainGridColor(1.0f, 1.0f, 1.0f), helperGridColor(0.5f, 0.5f, 0.5f), viewMatrix(1.0f), projMatrix(1.0f) {
		init();
	}

	void init() override;
	void render() override;

};