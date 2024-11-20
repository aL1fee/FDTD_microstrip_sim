#include "Cylinder_POT.h"

// TODO: all directions
void Cylinder_POT::buildVertices()
{
    // direction is hardcoded
    direction = __x_norm_vec3;
    float y = 0.0f, z = 0.0f;
    vertices->allocateNewEmptyArray();
    vertices->allocateNewEmptyArray();
    vertices->allocateNewEmptyArray();
    vertices->pushToIndArray(0, __zero_vec3);
    vertices->pushToIndArray(0, color);
    vertices->pushToIndArray(1, direction * length);
    vertices->pushToIndArray(1, color);
    for (float i = 0.0f; i < 360.0f + CYLINDER_DRAWING_ANGLE_INTERVAL;
        i += CYLINDER_DRAWING_ANGLE_INTERVAL)
    {
        z = -cos(glm::radians(i)) * radius;
        y = sin(glm::radians(i)) * radius;
        vertices->pushToIndArray(0, glm::vec3(0.0f, y, z));
        vertices->pushToIndArray(0, color);
        vertices->pushToIndArray(2, glm::vec3(0.0f, y, z));
        vertices->pushToIndArray(2, color);
        vertices->pushToIndArray(1, glm::vec3(0.0f
            + direction.x * length, y, z));
        vertices->pushToIndArray(1, color);
        vertices->pushToIndArray(2, glm::vec3(0.0f
            + direction.x * length, y, z));
        vertices->pushToIndArray(2, color);
    }
    if (edgesOn) {
        buildEdges();
    }
}

void Cylinder_POT::buildVAOs()
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

void Cylinder_POT::buildEdges()
{
    for (int k = 0; k < 2; k++)
    {
        vertices->allocateNewArray();
        for (float i = 0.0f; i < 360.0f + CYLINDER_DRAWING_ANGLE_INTERVAL;
            i += CYLINDER_DRAWING_ANGLE_INTERVAL)
        {
            float z = -cos(glm::radians(i)) * radius;
            float y = sin(glm::radians(i)) * radius;
            vertices->pushToExistingArray(glm::vec3(length * k,
                y, z));
            addColorVertex(glm::vec3(.1f));
        }
    }
}

void Cylinder_POT::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
    generateModelMatrix();
}

void Cylinder_POT::rebuild()
{
    generateModelMatrix();
}

void Cylinder_POT::generateModelMatrix()
{
    translationVector = origin;
    scalingVector = glm::vec3(length / initial_length,
        radius / initial_height, radius / initial_width);
    modelMatrix = glm::mat4(1.0f);
    if (!__centerOrientedRotation)
    {
        modelMatrix = glm::translate(modelMatrix, translationVector);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
        modelMatrix = glm::scale(modelMatrix, scalingVector);
    }
    // not implemented yet
    else { }
    inverseModelMatrix = glm::inverse(modelMatrix);
}


void Cylinder_POT::draw()
{
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }
    shader->bind();
    updateModelMatrix();
    for (int i = 0; i < 2; i++) {
        glBindVertexArray(VAOs->at(i));
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
        glBindVertexArray(0);
    }
    glBindVertexArray(VAOs->at(2));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(vertices->at(2)->size() / 2));
    glBindVertexArray(0);
    if (edgesOn) {
        for (int i = 3; i < 5; i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
            glBindVertexArray(0);
        }
    }
    shader->unbind();
}

bool Cylinder_POT::intersectionCheck(glm::vec3 v)
{
    glm::vec4 vLocalFrame = inverseModelMatrix * glm::vec4(v, 1.0f);
    if (0.0f < vLocalFrame.x && initial_length > vLocalFrame.x)
    {
        glm::vec3 diff = glm::vec3(0, vLocalFrame.y, vLocalFrame.z);
        if (glm::length(diff) > initial_width) {
            return false;
        }
        return true;
    }
    return false;
}

glm::vec3 Cylinder_POT::getCenterLocation() const
{
    glm::vec3 localCenter = glm::vec3((length / 2.0f) / scalingVector.x, 0.0f, 0.0f);
    return glm::vec3(modelMatrix * glm::vec4(localCenter, 1.0f));
}

void Cylinder_POT::setScaleH(float h)
{
    height = h;
    width = height;
    radius = height;
}

void Cylinder_POT::setScaleW(float w)
{
    width = w;
    height = width;
    radius = width;
}
