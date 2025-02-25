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
	std::vector<glm::vec3>* eX1D;
	std::vector<glm::vec3>* hY1D;
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

	VertexVectorDS* cellVerts1D;
	VAOVectorDS* cellVAOs1D;
	VertexVectorDS* cellVerts3D;
	VAOVectorDS* cellVAOs3D;

	VAOVectorDS* fields1DVAOs;

	int simulationDimension;
	bool oldSimulationRunning;

	void buildCellVertices1D();
	void buildCellVAOs1D();

	void buildCellVertices3D();
	void buildCellVAOs3D();

	void generateCells1D();
	void generateCells3D();

	void generateCells();

	void init();

	void drawCells();
	void deleteCells();

	bool needCellUpdate;
	bool renderingCellOn;

	bool simStopped;


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

	void initilizeFields()
	{
		initializeFields1D();
	}

	void setDimensions(glm::vec3 v) 
	{	simSpaceDimensions = v; 
		//initilizeFields();
		//updateFieldArraySizes();
	}

	glm::vec3* getDimensions() { return &simSpaceDimensions; }

	void setCellSize(float f) { cellSize = f; updateFieldArraySizes(); }
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

	void setRunning1D(bool b);
	void setDrawing1D(bool b) { drawing1D = b; }
	bool isDrawing1D() { return drawing1D; }

	float* getCWFrequency() { return &CWFrequency; }
	void setCWFrequency(float f) { CWFrequency = f; }

	int* getSlowdownFactor() { return &slowdownFactor; }
	void setSlowdownFactor(int i) { slowdownFactor = i; }

	//void reinitializeFields() { initializeFields1D(); }

	void updateFieldArraySizes();

	bool simulationRunning() { return running1D || running2D || running3D; }
	bool isOldSimulationRunning() { return oldSimulationRunning; }

	bool isSimStopped() { return simStopped; }
	void setSimStopped(bool b) { simStopped = b; }

};