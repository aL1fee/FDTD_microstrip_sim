#include "CoordinateSystem_PO.h"

CoordinateSystem_PO::CoordinateSystem_PO(float l, float mls, Shader* sh)
{
    vertices = new VertexVectorDS();
    VAOs = new VAOVectorDS();
    origin = glm::vec3(0.0f);
    length = l;
    width = l;
    height = l;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    permittivity = -1.0;
    conductivity = -1.0;
    rebuiltExpected = true;
    isInteractable = false;
    edgesOn = false;
    shader = sh;
    propertyMap = nullptr;
    helperLineSeparation = mls;
    mainColor = glm::vec3(0.0f);
    helperColor = glm::vec3(0.72f);
}

void CoordinateSystem_PO::buildVertices()
{
    vertices->pushToExistingArray(glm::vec3(-length / 2.0f, 0.0f, 0.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->pushToExistingArray(glm::vec3(length / 2.0f, 0.0f, 0.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->pushToExistingArray(glm::vec3(0.0f, -height / 2.0f, 0.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->pushToExistingArray(glm::vec3(0.0f, height / 2.0f, 0.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->pushToExistingArray(glm::vec3(0.0f, 0.0f, -width / 2.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->pushToExistingArray(glm::vec3(0.0f, 0.0f, width / 2.0f));
    vertices->pushToExistingArray(mainColor);
    vertices->allocateNewArray();
    int numLines = static_cast<int>(length / 2 / helperLineSeparation);
    for (int i = 1; i < numLines; i++) {
        vertices->pushToExistingArray(glm::vec3(i * helperLineSeparation, 0.0f, 0.0f));
        vertices->pushToExistingArray(helperColor);
        vertices->pushToExistingArray(glm::vec3(i * helperLineSeparation, 0.0f, (float)length / 2));
        vertices->pushToExistingArray(helperColor);
    }
    for (int i = 1; i < numLines; i++) {
        vertices->pushToExistingArray(glm::vec3(0.0f, 0.0f, i * helperLineSeparation));
        vertices->pushToExistingArray(helperColor);
        vertices->pushToExistingArray(glm::vec3((float)length / 2, 0.0f, i * helperLineSeparation));
        vertices->pushToExistingArray(helperColor);
    }
}

void CoordinateSystem_PO::buildVAOs()
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

void CoordinateSystem_PO::buildEdges() {}

void CoordinateSystem_PO::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void CoordinateSystem_PO::rebuild()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void CoordinateSystem_PO::draw()
{
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }
    shader->bind();
    for (int i = 0; i < vertices->getSize(); i++) {
        glBindVertexArray(VAOs->at(i));
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
        glBindVertexArray(0);
    }
    shader->unbind();
}

bool CoordinateSystem_PO::intersectionCheck(glm::vec3 v)
{
    return false;
}