#include "Cuboid_POT.h"

void Cuboid_POT::buildVertices()
{
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 5; i++) {
        z = (i == 1 || i == 2) ? 1 : 0;
        y = (i == 2 || i == 3) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, height * y, width * z));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(length, height * y, width * z));
        addColorVertex(color);
    }
    vertices->allocateNewArray();
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, height * y, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(0, height * y, width));
        addColorVertex(color);
    }
    vertices->allocateNewArray();
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(length, height * y, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(glm::vec3(length, height * y, width));
        addColorVertex(color);
    }
    addColorVertex(color);
    if (edgesOn) {
        buildEdges();
    }
}

void Cuboid_POT::buildVAOs()
{
    for (int i = 0; i < vertices->getSize(); i++) {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->at(i)->size(), vertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        VAOs->push(VAO);
    }
}

void Cuboid_POT::buildEdges()
{
    vertices->allocateNewArray();
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 4; i++) {
        z = (i == 1 || i == 2) ? 1 : 0;
        y = (i == 2 || i == 3) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, height * y, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(length, height * y, width * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, height * y, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(0, height * y, width));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(0, 0, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(0, height, width * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(length, height * y, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(length, height * y, width));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(glm::vec3(length, 0, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(glm::vec3(length, height, width * z));
        addColorVertex(glm::vec3(.1f));
    }
}

void Cuboid_POT::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
    generateModelMatrix();
}

void Cuboid_POT::rebuild()
{
    generateModelMatrix();
}

void Cuboid_POT::generateModelMatrix()
{
    translationVector = origin;
    scalingVector = glm::vec3(length / initial_length,
        height / initial_height, width / initial_width);
    modelMatrix = glm::mat4(1.0f);

    if (!__centerOrientedRotation)
    {
        modelMatrix = glm::translate(modelMatrix, translationVector);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
        modelMatrix = glm::scale(modelMatrix, scalingVector);
    }
    // not implemented yet
    else
    {
        glm::vec3 centerOffset = glm::vec3(length / scalingVector.x / 2.0f, height / scalingVector.y / 2.0f,
            width / scalingVector.z / 2.0f);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, translationVector);
        //modelMatrix = glm::translate(modelMatrix, centerOffset);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
        modelMatrix = glm::scale(modelMatrix, scalingVector);
        modelMatrix = glm::translate(modelMatrix, -centerOffset);
    }

    inverseModelMatrix = glm::inverse(modelMatrix);
}

void Cuboid_POT::draw()
{
    if (rebuiltExpected) 
    {
        rebuild();
        rebuiltExpected = false;
    }
    shader->bind();
    updateModelMatrix();

    if (VAOs->getSize() != 0)
    {
        for (int i = 0; i < 3; i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
            glBindVertexArray(0);
        }
        if (edgesOn) {
            for (int i = 3; i < 4; i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size()));
                glBindVertexArray(0);
            }
        }
    }
    shader->unbind();
}

bool Cuboid_POT::intersectionCheck(glm::vec3 v)
{
    glm::vec4 vLocalFrame = inverseModelMatrix * glm::vec4(v, 1.0f);
    if ((0.0f < vLocalFrame.x && length / scalingVector.x > vLocalFrame.x) &&
        (0.0f < vLocalFrame.y && height / scalingVector.y > vLocalFrame.y) &&
        (0.0f < vLocalFrame.z && width / scalingVector.z > vLocalFrame.z))
    {
        return true;
    }
    return false;   
}