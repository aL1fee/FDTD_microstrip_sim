#include "SimulationSpace.h"

SimulationSpace::SimulationSpace(GLFWwindow* w, MainScene* s)
{
	window = w;
	scene = s;
	cellColor = glm::vec3(.79f, .79f, .83f);
    cellOpaqueness = 1.0f;
	simSpaceDimensions = glm::vec3(4.5f, 2.0f, 7.0f);
	cellSize = .02f;
    simulationDimension = 3;
    cellShader = nullptr;
    needCellUpdate = false;
    renderingCellOn = false;
    cellVerts = new VertexVectorDS();
    cellVAOs = new VAOVectorDS();
    numCells1D = 200;
    cellSize1D = .02f;
    timeT = 0;
    running1D = false;
    fields1DVAOs = new VAOVectorDS();
    initialEFieldCol = glm::vec3(0.0f, 0.0f, 1.0f);
    initialHFieldCol = glm::vec3(1.0f, 0.0f, 0.0f);
    init();
    drawing1D = true;

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

void SimulationSpace::setFieldColors()
{
    unsigned int colLoc = eFieldShader->getUniformLocation("col");
    eFieldShader->bind();
    glm::vec4 v = glm::vec4(initialEFieldCol, 1.0f);
    glUniform4fv(colLoc, 1, glm::value_ptr(v));
    eFieldShader->unbind();
    unsigned int colLoc2 = hFieldShader->getUniformLocation("col");
    hFieldShader->bind();
    glm::vec4 v2 = glm::vec4(initialHFieldCol, 1.0f);
    glUniform4fv(colLoc2, 1, glm::value_ptr(v2));
    hFieldShader->unbind();
}

void SimulationSpace::setCellOpaqueness(float f)
{
    unsigned int colLoc = cellShader->getUniformLocation("col");
    cellShader->bind();
    glm::vec4 v = glm::vec4(cellColor, f);
    glUniform4fv(colLoc, 1, glm::value_ptr(v));
    cellShader->unbind();
}

void SimulationSpace::loadFieldShaders()
{
    std::string shaderEName = "e_field_shader";
    if (!scene->checkShader(shaderEName))
    {
        eFieldShader = new Shader("res/shaders/field.shader", 1, shaderEName);
        scene->addShader(eFieldShader);
    }
    else
    {
        eFieldShader = scene->getShader(shaderEName);
    }
    std::string shaderHName = "h_field_shader";
    if (!scene->checkShader(shaderHName))
    {
        hFieldShader = new Shader("res/shaders/field.shader", 1, shaderHName);
        scene->addShader(hFieldShader);
    }
    else
    {
        hFieldShader = scene->getShader(shaderHName);
    }
    setFieldColors();
}

void SimulationSpace::reset1D()
{
    running1D = false;
    fields1DVAOs->clear();
    initializeFields1D();
    timeT = 0;
    drawing1D = true;
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
    initializeFields1D();
    loadFieldShaders();
}

void SimulationSpace::drawCells()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);  // Disable writing to depth buffer
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
    glDepthMask(GL_TRUE);  // Re-enable depth writing
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
    if (running1D)
    {
        updateFields1D();
        buildFields1DVAOs();
    }


}

void SimulationSpace::render()
{
    update();
    drawCells();
    drawFields();
}

// do 1d first
// do another shader
void SimulationSpace::drawFields()
{
    if (drawing1D)
    {

        // might be less expensive to use a signle shader for both field
        // and change the color through uniforms
        eFieldShader->bind();
        glBindVertexArray(fields1DVAOs->at(0));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(numCells1D));
        glBindVertexArray(0);
        eFieldShader->unbind();
        hFieldShader->bind();
        glBindVertexArray(fields1DVAOs->at(1));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(numCells1D));
        glBindVertexArray(0);
        hFieldShader->unbind();

    }



}

void SimulationSpace::buildFields1DVAOs()
{
    fields1DVAOs->clear();

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numCells1D, eX1D, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(VBO);
    fields1DVAOs->push(VAO);

    unsigned int hVAO;
    glGenVertexArrays(1, &hVAO);
    glBindVertexArray(hVAO);
    unsigned int hVBO;
    glGenBuffers(1, &hVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numCells1D, hY1D, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(hVBO);
    fields1DVAOs->push(hVAO);

}

void SimulationSpace::initializeFields1D()
{
    for (int i = 0; i < numCells1D; i++)
    {
        eX1D[i] = glm::vec3(cellSize1D * i, 0.0f, 0.0f);
        hY1D[i] = glm::vec3(cellSize1D * i, 0.0f, 0.0f);
    }
    buildFields1DVAOs();
}

void SimulationSpace::updateFields1D()
{
    timeT++;
    int slowDownFactor = 3;
    if (timeT % slowDownFactor != 0)
    {
        return;
    }

    for (int i = 1; i < numCells1D; i++)
    {
        eX1D[i].y = eX1D[i].y + .5f * (hY1D[i - 1].z - hY1D[i].z);
    }
    float t0 = 40.0f;
    float spread = 3.0f;
    //hard source
    float pulse = (float) exp(-.5 * (pow((t0 - float (timeT / slowDownFactor)) / spread, 2.0)));
    eX1D[numCells1D / 2].y = pulse;


    //eX1D[80].y = pulse;
    //eX1D[130].y = pulse;
    for (int i = 1; i < numCells1D; i++)
    {
        hY1D[i].z = hY1D[i].z + .5f * (eX1D[i].y - eX1D[i + 1].y);
    }
}

