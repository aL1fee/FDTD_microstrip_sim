#include "Curve_POT.h"

void Curve_POT::buildVertices() {}

void Curve_POT::buildVAOs()
{
    for (int i = builtUntilIndex; i < vertices->getSize(); i++) {
        if (VAOs->getSize() < vertices->getSize()) {
            VAOs->push(0);
        }
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->at(i)->size(), vertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        VAOs->setAt(i, VAO);
    }
}

Curve_POT::Curve_POT(Shader* sh)
{
    objectType = TRACE;
    shader = sh;
    vertices = new VertexVectorDS();
    VAOs = new VAOVectorDS();
    length = 0;
    width = -1;
    height = -1;
    builtUntilIndex = 0;
}

void Curve_POT::build()
{
    buildVAOs();
}

void Curve_POT::rebuild()
{
    buildVAOs();
}

void Curve_POT::draw()
{
    rebuild();
    shader->bind();
    shader->setUniform3f("color", 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < VAOs->getSize(); i++) {
        glBindVertexArray(VAOs->at(i));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size()));
        glBindVertexArray(0);
    }
    shader->unbind();
}

//TODO
bool Curve_POT::intersectionCheck(glm::vec3 v)
{
    return false;
}

void Curve_POT::clear()
{
    vertices->clear();
    VAOs->clear();
    builtUntilIndex = 0;
}

void Curve_POT::addPoint(glm::vec3 v)
{
    vertices->pushToExistingArray(v);
}

void Curve_POT::terminateLine()
{
    if (vertices->getSize() != 0) {
        vertices->allocateNewArray();
        builtUntilIndex++;
    }
}

//not safe
void Curve_POT::terminateLineWithBuiltIndexIncr()
{
    if (vertices->getSize() != 0) {
        vertices->allocateNewArray();
    }
}





