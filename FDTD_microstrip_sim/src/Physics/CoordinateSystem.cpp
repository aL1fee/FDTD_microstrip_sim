#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem(float l, float mls)
{
    length = l;
    helperLineSeparation = mls;
    mainVerts = new std::array<glm::vec3, 6>;
    helperVerts = new std::vector<glm::vec3>;
    mainVAO = 0xffffffff, helperVAO = 0xffffffff;
    mainColor = glm::vec3(0.0f, 0.0f, 0.0f);
    helperColor = glm::vec3(0.72f, 0.72f, 0.72f);
}

void CoordinateSystem::buildMainVertices()
{
    mainVerts->at(0) = glm::vec3(-length / 2.0f, 0.0f, 0.0f);
    mainVerts->at(1) = glm::vec3(length / 2.0f, 0.0f, 0.0f);
    mainVerts->at(2) = glm::vec3(0.0f, -length / 2.0f, 0.0f);
    mainVerts->at(3) = glm::vec3(0.0f, length / 2.0f, 0.0f);
    mainVerts->at(4) = glm::vec3(0.0f, 0.0f, -length / 2.0f);
    mainVerts->at(5) = glm::vec3(0.0f, 0.0f, length / 2.0f);
}

void CoordinateSystem::buildMainVAO()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mainVerts->size(), mainVerts->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    mainVAO = VAO;
}

void CoordinateSystem::drawMain() const
{   
    glBindVertexArray(mainVAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}

void CoordinateSystem::buildHelperVertices()
{
    int numLines = length / 2 / helperLineSeparation;
    for (int i = 1; i < numLines; i++) {
        helperVerts->push_back(glm::vec3(i * helperLineSeparation, 0.0f, 0.0f));
        helperVerts->push_back(glm::vec3(i * helperLineSeparation, 0.0f, (float) length / 2));
    }
    for (int i = 1; i < numLines; i++) {
        helperVerts->push_back(glm::vec3(0.0f, 0.0f, i * helperLineSeparation));
        helperVerts->push_back(glm::vec3((float)length / 2, 0.0f, i * helperLineSeparation));
    }
}

void CoordinateSystem::buildHelperVAO()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * helperVerts->size(), helperVerts->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    helperVAO = VAO;
}

void CoordinateSystem::drawHelper() const
{
    glBindVertexArray(helperVAO);
    glDrawArrays(GL_LINES, 0, helperVerts->size());
    glBindVertexArray(0);
}

void CoordinateSystem::run()
{
    buildMainVertices();
    buildMainVAO();
    buildHelperVertices();
    buildHelperVAO();
}
