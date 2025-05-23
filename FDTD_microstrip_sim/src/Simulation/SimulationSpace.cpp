#include "SimulationSpace.h"

extern MainScene* _scene_main;

SimulationSpace::SimulationSpace(GLFWwindow* w, MainScene* s)
{
	window = w;
	scene = s;
    eX1D = new std::vector<glm::vec3>();
    hY1D = new std::vector<glm::vec3>();
    eX1DHelperLines1D = new std::vector<glm::vec3>();
    hY1DHelperLines1D = new std::vector<glm::vec3>();
	cellColor = glm::vec3(.79f, .79f, .83f);
    oldSimulationRunning = false;
    cellOpaqueness = .35f;
	simSpaceDimensions = glm::vec3(4.5f, 2.0f, 7.0f);
	cellSize = .02f;
    simulationDimension = 1;
    cellShader = nullptr;
    CWFrequency = 2.0f;
    needCellUpdate = false;
    renderingCellOn = false;
    cellVerts3D = new VertexVectorDS();
    cellVAOs3D = new VAOVectorDS();
    cellVerts1D = new VertexVectorDS();
    cellVAOs1D = new VAOVectorDS();
    numCells1D = 200;
    cellSize1D = .02f;
    timeT = 0;
    simStopped = false;
    running1D = false;
    fields1DVAOs = new VAOVectorDS();
    initialEFieldCol = glm::vec3(0.0f, 0.0f, 1.0f);
    initialHFieldCol = glm::vec3(1.0f, 0.0f, 0.0f);
    init();
    slowdownFactor = 3;
    drawing1D = true;
    powerSources = new std::vector<PowerSource_PO*>();
    helperFieldLines1DOn = false;
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
    simStopped = false;
    running1D = false;
    fields1DVAOs->clear();
    initializeFields1D();
    timeT = 0;
    drawing1D = true;
    oldSimulationRunning = false;
}

void SimulationSpace::setRunning1D(bool b)
{
    if (!oldSimulationRunning || simStopped)
    {
        running1D = b;
        oldSimulationRunning = true;
        if (b && !simStopped)
        {
            initializeFields1D();
        }
        else
        {
            if (b)
            {
                simStopped = false;
            }
        }
    }
}

//void SimulationSpace::updateFieldArraySizes()
//{
//    for (int i = 0; i < eX1D->size() - 
//        static_cast<int>(simSpaceDimensions.x / cellSize); i++)
//    {
//        eX1D->push_back(glm::vec3(0.0f));
//        hY1D->push_back(glm::vec3(0.0f));
//    }
//    buildFields1DVAOs();
//}

void SimulationSpace::buildCellVertices1D()
{
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        cellVerts1D->pushToExistingArray(glm::vec3(0, -1.0f, -1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(0, 1.0f, -1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(0, 1.0f, 1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(0, -1.0f, 1.0));
    }
    cellVerts1D->allocateNewArray();
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        cellVerts1D->pushToExistingArray(glm::vec3(simSpaceDimensions.x, -1.0f, -1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(simSpaceDimensions.x, 1.0f, -1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(simSpaceDimensions.x, 1.0f, 1.0));
        cellVerts1D->pushToExistingArray(glm::vec3(simSpaceDimensions.x, -1.0f, 1.0));
    }
    cellVerts1D->allocateNewArray();
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        cellVerts1D->pushToExistingArray(glm::vec3(i * cellSize, -1.0f, 0));
        cellVerts1D->pushToExistingArray(glm::vec3(i * cellSize, 1.0f, 0));
    }
}

void SimulationSpace::buildCellVAOs1D()
{
    for (int i = 0; i < cellVerts1D->getSize(); i++) {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cellVerts1D->at(i)->size(), cellVerts1D->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        cellVAOs1D->addVBO(VBO);
        cellVAOs1D->push(VAO);
    }
}

void SimulationSpace::buildCellVertices3D()
{
	//cellVerts = new VertexVectorDS();

    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.y / cellSize); j++)
        {
            cellVerts3D->pushToExistingArray(glm::vec3(i * cellSize, j * cellSize, 0));
            cellVerts3D->pushToExistingArray(glm::vec3(i * cellSize, j * cellSize,
                cellSize * static_cast<int>((simSpaceDimensions.z / cellSize))));
        }
    }
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.z / cellSize); j++)
        {
            cellVerts3D->pushToExistingArray(glm::vec3(i * cellSize, 0.0f, j * cellSize));
            cellVerts3D->pushToExistingArray(glm::vec3(i * cellSize, cellSize *
                static_cast<int>((simSpaceDimensions.y / cellSize)),
                j * cellSize));
        }
    }
    for (int i = 0; i <= static_cast<int>(simSpaceDimensions.y / cellSize); i++)
    {
        for (int j = 0; j <= static_cast<int>(simSpaceDimensions.z / cellSize); j++)
        {
            cellVerts3D->pushToExistingArray(glm::vec3(0.0f, i * cellSize, j * cellSize));
            cellVerts3D->pushToExistingArray(glm::vec3(cellSize *
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

void SimulationSpace::buildCellVAOs3D()
{
	//cellVAOs = new VAOVectorDS();
    for (int i = 0; i < cellVerts3D->getSize(); i++) {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cellVerts3D->at(i)->size(), cellVerts3D->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        cellVAOs3D->addVBO(VBO);
        cellVAOs3D->push(VAO);
    }
}

void SimulationSpace::generateCells1D()
{
    buildCellVertices1D();
    buildCellVAOs1D();
}

void SimulationSpace::generateCells3D()
{
    buildCellVertices3D();
    buildCellVAOs3D();
}

void SimulationSpace::generateCells()
{
    generateCells3D(); //shader created here
    generateCells1D();
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
        if (simulationDimension == 1)
        {
            //glDepthMask(GL_TRUE);
            for (int i = 0; i < 2; i++) {
                glBindVertexArray(cellVAOs1D->at(i));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(cellVerts1D->at(i)->size()));
                glBindVertexArray(0);
            }
            //glDepthMask(GL_FALSE);
            glBindVertexArray(cellVAOs1D->at(2));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(cellVerts1D->at(2)->size()));
            glBindVertexArray(0);
        }
        if (simulationDimension == 3)
        {
            for (int i = 0; i < cellVerts3D->getSize(); i++) {
                glBindVertexArray(cellVAOs3D->at(i));
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(cellVerts3D->at(i)->size()));
                glBindVertexArray(0);
            }
        }
        cellShader->unbind();
    }
    glDepthMask(GL_TRUE);  // Re-enable depth writing
    glDisable(GL_BLEND);
}

void SimulationSpace::deleteCells()
{
    cellVerts1D->clear();
    cellVAOs1D->clear();
    cellVerts3D->clear();
    cellVAOs3D->clear();
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
    if (!oldSimulationRunning)
    {
        return;
    }
    if (drawing1D)
    {

        // might be less expensive to use a signle shader for both field
        // and change the color through uniforms
        eFieldShader->bind();
        glBindVertexArray(fields1DVAOs->at(0));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(static_cast<int>(simSpaceDimensions.x / cellSize)));
        glBindVertexArray(0);

        if (helperFieldLines1DOn)
        {
            glBindVertexArray(fields1DVAOs->at(1));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(2 * static_cast<int>(simSpaceDimensions.x / cellSize)));
            glBindVertexArray(0);
        }

        eFieldShader->unbind();
        hFieldShader->bind();
        glBindVertexArray(fields1DVAOs->at(2));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(static_cast<int>(simSpaceDimensions.x / cellSize)));
        glBindVertexArray(0);

        if (helperFieldLines1DOn)
        {
            glBindVertexArray(fields1DVAOs->at(3));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(2 * static_cast<int>(simSpaceDimensions.x / cellSize)));
            glBindVertexArray(0);
        }

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * eX1D->size(), eX1D->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(VBO);
    fields1DVAOs->push(VAO);

    unsigned int VAOHelperLines1D;
    glGenVertexArrays(1, &VAOHelperLines1D);
    glBindVertexArray(VAOHelperLines1D);
    unsigned int VBOHelperLines1D;
    glGenBuffers(1, &VBOHelperLines1D);
    glBindBuffer(GL_ARRAY_BUFFER, VBOHelperLines1D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * eX1DHelperLines1D->size(), eX1DHelperLines1D->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(VBOHelperLines1D);
    fields1DVAOs->push(VAOHelperLines1D);


    unsigned int hVAO;
    glGenVertexArrays(1, &hVAO);
    glBindVertexArray(hVAO);
    unsigned int hVBO;
    glGenBuffers(1, &hVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hY1D->size(), hY1D->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(hVBO);
    fields1DVAOs->push(hVAO);

    unsigned int hVAOHelperLines1D;
    glGenVertexArrays(1, &hVAOHelperLines1D);
    glBindVertexArray(hVAOHelperLines1D);
    unsigned int hVBOHelperLines1D;
    glGenBuffers(1, &hVBOHelperLines1D);
    glBindBuffer(GL_ARRAY_BUFFER, hVBOHelperLines1D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hY1DHelperLines1D->size(), hY1DHelperLines1D->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    fields1DVAOs->addVBO(hVBOHelperLines1D);
    fields1DVAOs->push(hVAOHelperLines1D);


}

void SimulationSpace::initializeFields1D()
{
    delete powerSources;
    powerSources = _scene_main->getPowerSources();


    //std::cout << static_cast<int>(simSpaceDimensions.x / cellSize) << std::endl;
    eX1D->clear();
    hY1D->clear();
    eX1DHelperLines1D->clear();
    hY1DHelperLines1D->clear();

    for (int i = 0; i < static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        eX1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
        hY1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
        eX1DHelperLines1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
        eX1DHelperLines1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
        hY1DHelperLines1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
        hY1DHelperLines1D->push_back(glm::vec3(cellSize * i, 0.0f, 0.0f));
       /* eX1D->at(i) = glm::vec3(cellSize * i, 0.0f, 0.0f);
        hY1D->at(i) = glm::vec3(cellSize * i, 0.0f, 0.0f);*/
    }
    //std::cout << "eX1D, hY1D: " << eX1D->size() << ", " << hY1D->size() << std::endl;


    buildFields1DVAOs();

}

void SimulationSpace::updateFields1D()
{
    timeT++;
    if (timeT % slowdownFactor != 0)
    {
        return;
    }

    for (int i = 1; i < static_cast<int>(simSpaceDimensions.x / cellSize); i++)
    {
        eX1D->at(i).y = eX1D->at(i).y + .5f * (hY1D->at(i - 1).z - hY1D->at(i).z);
        eX1DHelperLines1D->at(i * 2).y = eX1D->at(i).y;
    }
    float t0 = 40.0f;
    float spread = 3.0f;
    float pulse = (float) exp(-.5 * (pow((t0 - float (timeT / slowdownFactor)) / spread, 2.0)));
    pulse = sin(2 * 3.141592f * 1000000000 * timeT * .00000000001f);

    int psPosIndex = 0;

    for (PowerSource_PO* ps : *powerSources)
    {
        for (int i = 1; i < static_cast<int>(simSpaceDimensions.x / cellSize); i++)
        {
            if (ps->intersectionCheck(eX1D->at(i)))
            {
                if (*ps->getRotationAngle() > 90.0f &&
                    *ps->getRotationAngle() < 270.0f)
                {
                    float psPos = ps->getOrigin()->x;
                    psPosIndex = static_cast<int>(psPos / cellSize);

                    if (psPosIndex < static_cast<int>(simSpaceDimensions.x / cellSize))
                    {
                        pulse = sin(2 * (float) M_PI * *ps->getFrequency() * GHZ_TO_HZ *timeT * .00000000001f);
                        eX1D->at(psPosIndex).y += pulse;
                        break;
                    }


                }
                else {
                    float psPos = *ps->getLength() + ps->getOrigin()->x;
                    psPosIndex = static_cast<int>(psPos / cellSize);
                    if (psPosIndex < static_cast<int>(simSpaceDimensions.x / cellSize))
                    {
                        eX1D->at(psPosIndex).y += pulse;
                        break;
                    }


                    //pulse = sin(2 * (float) M_PI * *ps->getFrequency() * GHZ_TO_HZ * timeT * .0000000001f);
                    //eX1D->at(i).y += pulse;

                    //float epsilon_eff = 2.2; 
                    //float waveSpeed = SPEED_OF_LIGHT / sqrt(epsilon_eff);

                    //float k = 2 * M_PI * *ps->getFrequency() * GHZ_TO_HZ / waveSpeed; // Wave number
                    // pulse = sin(2 * (float)M_PI * .5f * GHZ_TO_HZ * timeT * 1e-11f);                        pulse = sin(2 * (float)M_PI * *ps->getFrequency() * GHZ_TO_HZ * timeT * 1e-11f - k * float(i) * cellSize);
                    //pulse = sin(2 * 3.141592f * 100000000 * timeT * .00000000001f);

                    //eX1D->at(i).y += pulse;
                    //break;

                }
            }
        }
    }



    //eX1D[80].y = pulse;
    //eX1D[130].y = pulse;
    for (int i = 0; i < static_cast<int>(simSpaceDimensions.x / cellSize) - 1; i++)
    {
        hY1D->at(i).z = hY1D->at(i).z + .5f * (eX1D->at(i).y - eX1D->at(i + 1).y);
        hY1DHelperLines1D->at(i * 2).z = hY1D->at(i).z;
    }

    //hY1D->at(psPosIndex).z += pulse;

    //hY1D->at(100).z -= pulse;

    //// PEC
    //eX1D[0] = glm::vec3(0.0f);
    //// PMC
    //hY1D[numCells1D - 1] = glm::vec3(0.0f);

}

