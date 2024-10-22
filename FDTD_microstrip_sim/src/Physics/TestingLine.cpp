#include "TestingLine.h"

void TestingLine::buildVertices()
{

}

void TestingLine::buildVAOs()
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

TestingLine::TestingLine()
{
    vertices = new VertexVectorDS();
    VAOs = new VAOVectorDS();
    builtUntilIndex = 0;
}

void TestingLine::build()
{
    buildVAOs();
}

void TestingLine::draw() const
{
    for (int i = 0; i < VAOs->getSize(); i++) {
        glBindVertexArray(VAOs->at(i));
        glDrawArrays(GL_LINE_STRIP, 0, vertices->at(i)->size());
        glBindVertexArray(0);
    }
}

void TestingLine::addPoint(glm::vec3 v)
{
    vertices->pushToExistingArray(v);
}

void TestingLine::terminateLine()
{
    vertices->allocateNewArray();
    builtUntilIndex++;
}





