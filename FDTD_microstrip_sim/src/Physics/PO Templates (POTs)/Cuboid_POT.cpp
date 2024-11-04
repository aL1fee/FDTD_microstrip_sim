#include "Cuboid_POT.h"

void Cuboid_POT::buildVertices()
{
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 5; i++) {
        z = (i == 1 || i == 2) ? 1 : 0;
        y = (i == 2 || i == 3) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, width * z));
        addColorVertex(color);
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, width * z));
        addColorVertex(color);
    }
    vertices->allocateNewArray();
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, width));
        addColorVertex(color);
    }
    vertices->allocateNewArray();
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, 0));
        addColorVertex(color);
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, width));
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
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, width * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(origin + glm::vec3(0, height * y, width));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(0, 0, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(origin + glm::vec3(0, height, width * z));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        y = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, 0));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(origin + glm::vec3(length, height * y, width));
        addColorVertex(glm::vec3(.1f));
    }
    for (int i = 0; i < 2; i++) {
        z = (i == 1) ? 1 : 0;
        vertices->pushToExistingArray(origin + glm::vec3(length, 0, width * z));
        addColorVertex(glm::vec3(.1f));
        vertices->pushToExistingArray(origin + glm::vec3(length, height, width * z));
        addColorVertex(glm::vec3(.1f));
    }
}

void Cuboid_POT::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void Cuboid_POT::rebuild()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void Cuboid_POT::draw()
{
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }
    shader->bind();
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
    shader->unbind();
}

bool Cuboid_POT::intersectionCheck(glm::vec3 v)
{
    if ((origin.x < v.x && origin.x + length > v.x) &&
        (origin.y < v.y && origin.y + height > v.y) &&
        (origin.z < v.z && origin.z + width > v.z))
    {
        return true;
    }
    return false;   
}