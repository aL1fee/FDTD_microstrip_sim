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

	//hardcodes for now
	int numCells1D;
	glm::vec3 eX1D[200];
	glm::vec3 hY1D[200];
	float cellSize1D;

	int timeT; //arbitrary time
	int timeS;
	int timeMS;
	int timeNS;

	//do enums instead
	bool running1D;
	bool running2D;
	bool running3D;

	bool drawing1D;

	float CWFrequency;

	Shader* cellShader;
	Shader* eFieldShader;
	Shader* hFieldShader;

	glm::vec3 initialEFieldCol;
	glm::vec3 initialHFieldCol;

	int slowdownFactor;

	VertexVectorDS* cellVerts;
	VAOVectorDS* cellVAOs;

	VAOVectorDS* fields1DVAOs;

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
	void setFieldColors();
	void setCellOpaqueness(float f);

	void update();
	void render();
	void drawFields();

	void buildFields1DVAOs();

	void initializeFields1D();
	void updateFields1D();

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

	void loadFieldShaders();

	//do enums instead for 1d, 2d, 3d
	void reset1D();

	void setRunning1D(bool b) { running1D = b; }
	void setDrawing1D(bool b) { drawing1D = b; }
	bool isDrawing1D() { return drawing1D; }

	float* getCWFrequency() { return &CWFrequency; }
	void setCWFrequency(float f) { CWFrequency = f; }

	int* getSlowdownFactor() { return &slowdownFactor; }
	void setSlowdownFactor(int i) { slowdownFactor = i; }

};