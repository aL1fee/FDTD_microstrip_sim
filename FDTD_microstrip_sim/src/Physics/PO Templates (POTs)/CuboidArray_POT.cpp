#include "CuboidArray_POT.h"

void CuboidArray_POT::setInstancePositions()
{
    for (int countZ = 0; countZ < numPadsZ; countZ++)
    {
        for (int countX = 0; countX < numPadsX; countX++)
        {
            glm::vec3 translation = glm::vec3(countX * (sizeX + padSeparationX), 0.0f, countZ * (sizeZ + padSeparationZ));
            instancePositions->push_back(translation);
        }
    }
}

void CuboidArray_POT::buildVertices()
{
    vertices->allocateNewArray();
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 5; i++) {
        z = (i == 1 || i == 2) ? 1 : 0;
        y = (i == 2 || i == 3) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, y * height, 1 * z));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(1, y * height, 1 * z));
        addColorVertex(color);
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, y * height, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(0, y * height, 1));
        addColorVertex(color);
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(1, y * height, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(1, y * height, 1));
        addColorVertex(color);
    }
    if (edgesOn) {
        buildEdges();
    }
    setInstancePositions();
}

void CuboidArray_POT::buildVAOs()
{
    for (int i = 0; i < vertices->getSize(); i++)
    {
        unsigned int VAO, VBO;

        // Generate a VAO and bind
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Create the VBO for the cuboid vertices
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->at(i)->size(), vertices->at(i)->data(), GL_STATIC_DRAW);


        // Set vertex attribute pointers (position, color, etc.)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));  // Color
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Generate a VBO for the instance positions
        glGenBuffers(1, &(instanceVBO[i]));
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, MAX_NUM_PADS * MAX_NUM_PADS * sizeof(glm::vec3), 
            nullptr, GL_STATIC_DRAW);
        /*glBufferSubData(GL_ARRAY_BUFFER, 0, instancePositions->size() * sizeof(glm::vec3), 
            instancePositions->data());*/
        
        // Set the instance positions in attribute 2
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        // Tell OpenGL that this attribute is an instanced attribute
        glVertexAttribDivisor(2, 1);  // Attribute 2 will change per instance, not per vertex

        VAOs->push(VAO);
    }
}

void CuboidArray_POT::buildEdges()
{
    vertices->allocateNewArray();
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 4; i++) {
        z = (i == 1 || i == 2) ? 1 : 0;
        y = (i == 2 || i == 3) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, y * height, 1 * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(1, y * height, 1 * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, y * height, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(0, y * height, 1));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, 0, 1 * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(0, height, 1 * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(1, y * height, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(1, y * height, 1));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(1, 0, 1 * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(1, height, 1 * z));
        addColorVertex(glm::vec3(.1f));
    }
}

void CuboidArray_POT::build()
{
    VAOs->clear();
    buildVertices();
    buildVAOs();
    generateModelMatrix();
    //rebuiltExpected = true;
}

void CuboidArray_POT::rebuild()
{
    generateModelMatrix();
}

void CuboidArray_POT::generateModelMatrix()
{
    if (instancePositionsChanged)
    {
        instancePositions->clear();
        setInstancePositions();
        for (int i = 0; i < vertices->getSize(); i++)
        {
            glBindVertexArray(VAOs->at(i));
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[i]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                instancePositions->size() * sizeof(glm::vec3),
                instancePositions->data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        instancePositionsChanged = false;
    }

    translationVector = origin;
    scalingVector = glm::vec3(1.0f, height / initial_height,
       1.0f);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translationVector);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    
    inverseModelMatrix = glm::inverse(modelMatrix);
    
    modelMatrix = glm::scale(modelMatrix, scalingVector);
}

void CuboidArray_POT::draw()
{
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }

    shader->bind();

    updateModelMatrix();

    shader->setUniform1f("sizeX", sizeX);
    shader->setUniform1f("sizeZ", sizeZ);

    glBindVertexArray(VAOs->at(0));
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0,
        static_cast<GLsizei>(vertices->at(0)->size() / 2),
        numPadsX * numPadsZ);
    glBindVertexArray(0);

    if (edgesOn) {
        glBindVertexArray(VAOs->at(1));
        glDrawArraysInstanced(GL_LINES, 0,
            static_cast<GLsizei>(vertices->at(1)->size() / 2),
            numPadsX * numPadsZ);
        glBindVertexArray(0);
    }
    shader->unbind();
}

bool CuboidArray_POT::intersectionCheck(glm::vec3 v)
{
    v = inverseModelMatrix * glm::vec4(v, 1.0f);
    for (int countZ = 0; countZ < numPadsZ; countZ++)
    {
        for (int countX = 0; countX < numPadsX; countX++)
        {
            if (0.0f > v.y || height < v.y)
            {
                return false;
            }
            if ((countX * sizeX + countX * padSeparationX < v.x) &&
                (countX * sizeX + countX * padSeparationX + sizeX > v.x) &&
                (countZ * sizeZ + countZ * padSeparationZ < v.z) &&
                (countZ * sizeZ + countZ * padSeparationZ + sizeZ > v.z))
            {
                return true;
            }
        }
    }
    return false;
}

void CuboidArray_POT::setScaleL(float l)
{
    float totalPadSeparation = l - sizeX * numPadsX;
    padSeparationX = totalPadSeparation / (numPadsX - 1);
    if (padSeparationX < MIN_PAD_SEPARATION_SIZE) {
        padSeparationX = MIN_PAD_SEPARATION_SIZE;
    }
    else if (padSeparationX > MAX_PAD_SEPARATION_SIZE) {
        padSeparationX = MAX_PAD_SEPARATION_SIZE;
    }
    length = sizeX * numPadsX + padSeparationX * (numPadsX - 1);
}

void CuboidArray_POT::setScaleW(float w)
{
    float totalPadSeparation = w - sizeZ * numPadsZ;
    padSeparationZ = totalPadSeparation / (numPadsZ - 1);
    if (padSeparationZ < MIN_PAD_SEPARATION_SIZE) {
        padSeparationZ = MIN_PAD_SEPARATION_SIZE;
    }
    else if (padSeparationZ > MAX_PAD_SEPARATION_SIZE) {
        padSeparationZ = MAX_PAD_SEPARATION_SIZE;
    }
    width = sizeZ * numPadsZ + padSeparationZ * (numPadsZ - 1);
}


void CuboidArray_POT::updateL()
{
    length = sizeX * numPadsX + padSeparationX * (numPadsX - 1);
}

void CuboidArray_POT::updateW()
{
    width = sizeZ * numPadsZ + padSeparationZ * (numPadsZ - 1);
}