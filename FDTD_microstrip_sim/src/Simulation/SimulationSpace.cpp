#include "SimulationSpace.h"

SimulationSpace::SimulationSpace(GLFWwindow* w, MainScene* s)
{
	window = w;
	scene = s;
	cellColor = glm::vec3(.79f, .79f, .83f);
    cellOpaqueness = 1.0f;
	simSpaceDimensions = glm::vec3(4.5f, 2.0f, 7.0f);
	cellSize = .2f;
    cellShader = nullptr;
    needCellUpdate = false;
    renderingCellOn = false;
    cellVerts = new VertexVectorDS();
    cellVAOs = new VAOVectorDS();
	init();
}

SimulationSpace::~SimulationSpace()
{

}

void SimulationSpace::setCellColor(glm::vec3 c)
{
    unsigned int colLoc = cellShader->getUniformLocation("col");
    cellShader->bind();
    glm::vec4 v = glm::vec4(c, cellOpaqueness);
    glUniform4fv(colLoc, 1, glm::value_ptr(v));
    cellShader->unbind();
}

void SimulationSpace::setCellOpaqueness(float f)
{
    unsigned int colLoc = cellShader->getUniformLocation("col");
    cellShader->bind();
    glm::vec4 v = glm::vec4(cellColor, f);
    glUniform4fv(colLoc, 1, glm::value_ptr(v));
    cellShader->unbind();
}

void SimulationSpace::buildCellVertices()
{
	//cellVerts = new VertexVectorDS();

    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.y / cellSize); j++)
        {
            cellVerts->pushToExistingArray(glm::vec3(i * cellSize, j * cellSize, 0));
            cellVerts->pushToExistingArray(glm::vec3(i * cellSize, j * cellSize, 
                cellSize * static_cast<int>((simSpaceDimensions.z / cellSize))));
        }
    }
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.z / cellSize); j++)
        {
            cellVerts->pushToExistingArray(glm::vec3(i * cellSize, 0.0f, j * cellSize));
            cellVerts->pushToExistingArray(glm::vec3(i * cellSize, cellSize * 
                static_cast<int>((simSpaceDimensions.y / cellSize)),
                j * cellSize));
        }
    }
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.y / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.z / cellSize); j++)
        {
            cellVerts->pushToExistingArray(glm::vec3(0.0f, i * cellSize, j * cellSize));
            cellVerts->pushToExistingArray(glm::vec3(cellSize *
                static_cast<int>((simSpaceDimensions.x / cellSize)), i * cellSize,
                j * cellSize));
        }
    }
    std::string shaderName = "simulation_cells";
    if (!scene->checkShader(shaderName))
    {
        cellShader = new Shader("res/shaders/simulation_cells.shader", 1, shaderName);
        scene->addShader(cellShader);
    }
    else
    {
        cellShader = scene->getShader(shaderName);
    }
    setCellColor(cellColor);
}

void SimulationSpace::buildCellVAOs()
{
	//cellVAOs = new VAOVectorDS();
    for (int i = 0; i < cellVerts->getSize(); i++) {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cellVerts->at(i)->size(), cellVerts->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        cellVAOs->addVBO(VBO);
        cellVAOs->push(VAO);
    }
}

void SimulationSpace::generateCells()
{
	buildCellVertices();
	buildCellVAOs();
}

void SimulationSpace::init()
{
	generateCells();
}

void SimulationSpace::drawCells()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (renderingCellOn)
    {
        cellShader->bind();
        for (int i = 0; i < cellVerts->getSize(); i++) {
            glBindVertexArray(cellVAOs->at(i));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(cellVerts->at(i)->size()));
            glBindVertexArray(0);
        }
        cellShader->unbind();
    }
    glDisable(GL_BLEND);
}

void SimulationSpace::deleteCells()
{
    cellVerts->clear();
    cellVAOs->clear();
    //delete cellVerts;
    //delete cellVAOs;
}

void SimulationSpace::update()
{
    if (needCellUpdate)
    {
        deleteCells();
        generateCells();
        needCellUpdate = false;
    }



}

void SimulationSpace::render()
{
    update();
    drawCells();
}

