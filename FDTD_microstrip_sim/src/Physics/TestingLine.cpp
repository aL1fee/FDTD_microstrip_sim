#include "TestingLine.h"

void TestingLine::buildVertices()
{

}

void TestingLine::buildVAOs()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->size(), vertices->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    if (VAOs->size() != 0) 
    {
        VAOs->front() = VAO;
    }
    else {
        VAOs->push_back(VAO);
    }
}

TestingLine::TestingLine()
{
    vertices = new std::vector<glm::vec3>();
    VAOs = new std::vector<unsigned int>();
}

std::vector<glm::vec3>* TestingLine::build()
{
    buildVAOs();
    return nullptr;
}

void TestingLine::draw() const
{
    if (VAOs->size() != 0)
    {
        glBindVertexArray(VAOs->at(0));
        glDrawArrays(GL_LINE_STRIP, 0, vertices->size());
        glBindVertexArray(0);
    }
}

void TestingLine::addPoint(glm::vec3 v)
{
    vertices->push_back(v);
}
