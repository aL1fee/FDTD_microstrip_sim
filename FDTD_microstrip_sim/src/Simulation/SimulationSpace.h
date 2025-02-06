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

#include "../Data Structures/VAOVectorDS.h"
#include "../Data Structures/VertexVectorDS.h"
#include "../Scene/Scene.h"
#include "../Scene/MainScene.h"

class SimulationSpace
{
protected:
	GLFWwindow* window;
	MainScene* scene;

	//x, y, z
	glm::vec3 simSpaceDimensions;
	glm::vec3 cellColor;
	float cellOpaqueness;
	float cellSize;

	Shader* cellShader;
	VertexVectorDS* cellVerts;
	VAOVectorDS* cellVAOs;

	int simulationDimension;


	void buildCellVertices();
	void buildCellVAOs();
	void generateCells();

	void init();

	void drawCells();

	void deleteCells();

	bool needCellUpdate;
	bool renderingCellOn;

public:
	SimulationSpace(GLFWwindow* w, MainScene* s);
	~SimulationSpace();

	void setCellColor(glm::vec3 c);
	void setCellOpaqueness(float f);

	void update();
	void render();

	void setDimensions(glm::vec3 v) { simSpaceDimensions = v; }
	glm::vec3* getDimensions() { return &simSpaceDimensions; }

	void setCellSize(float f) { cellSize = f; }
	float* getCellSize() { return &cellSize; }

	float* getCellOpaqueness() { return &cellOpaqueness; }

	void setCellUpdate(bool b) { needCellUpdate = b; }
	void setRenderingCell(bool b) { renderingCellOn = b; }

	bool getRenderingCellOn() { return renderingCellOn; }

	glm::vec3* getCellColor() { return &cellColor; }

	int* getSimulationDimension() { return &simulationDimension; }

	void setSimulationDimension(int i) { simulationDimension = i; }


};