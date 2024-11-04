#include "DimensionalCurve_POT.h"

// process vertices 
void DimensionalCurve_POT::buildVertices()
{
    if (processedVertices.size() == 0 || unprocessedVertices.size() == 0)
    {
        return;
    }
    glm::vec3 lastUnprocessedVertex = 
        unprocessedVertices.at(unprocessedVertices.size() - 1);
    glm::vec3 lastProcessedVertex = processedVertices.at(processedVertices.size() - 1);
    float lengthSinceLastProcessedVertex = 
        glm::length(lastUnprocessedVertex - lastProcessedVertex);
    if (lengthSinceLastProcessedVertex > DIMENSIONAL_CURVE_DELTA_LENGTH)
    {
        glm::vec3 normalDirVec = glm::normalize(lastUnprocessedVertex - lastProcessedVertex);
        glm::vec3 newProcessedVec = lastProcessedVertex + DIMENSIONAL_CURVE_DELTA_LENGTH * normalDirVec;
        processedVertices.push_back(newProcessedVec);


    }
}

void DimensionalCurve_POT::buildVAOs()
{

    VAOs->clear();

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * processedVertices.size(), processedVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    VAOs->push(VAO);
   




    //for (int i = 0; i < processedVertices.size(); i++) {
    //    if (VAOs->getSize() < processedVertices.size()) {
    //        VAOs->push(0);
    //    }
    //    unsigned int VAO;
    //    glGenVertexArrays(1, &VAO);
    //    glBindVertexArray(VAO);
    //    unsigned int VBO;
    //    glGenBuffers(1, &VBO);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * processedVertices.size(), &processedVertices.at(i), GL_STATIC_DRAW);
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //    glEnableVertexAttribArray(0);
    //    //VAOs->setAt(i, VAO);
    //    std::cout << "VAOs.size(): " <<
    //        VAOs->getSize() << std::endl;
    //    VAOs->push(VAO);
    //}
}

void DimensionalCurve_POT::buildEdges()
{

}

void DimensionalCurve_POT::rebuild()
{
    buildVertices();
    buildVAOs();
}

void DimensionalCurve_POT::addPoint(glm::vec3 v)
{
    if (processedVertices.size() == 0)
    {
        processedVertices.push_back(v);
        indexProcessed++;
    }
    unprocessedVertices.push_back(v);
}

void DimensionalCurve_POT::build()
{

}

void DimensionalCurve_POT::draw()
{
    //std::cout << "start" << std::endl;
    //for (int i = 0; i < processedVertices.size(); i++)
    //{
    //    std::cout << glm::to_string(processedVertices[i]) << std::endl;
    //}
    //std::cout << "finish" << std::endl;

    rebuild();
    shader->bind();
    shader->setUniform3f("color", color.x, color.y, color.z);

    glBindVertexArray(VAOs->at(0));
    glDrawArrays(GL_LINE_STRIP, 0,
        static_cast<GLsizei>(processedVertices.size()));
    glBindVertexArray(0);


    //for (int i = 0; i < VAOs->getSize(); i++) {
    //    glBindVertexArray(VAOs->at(i));
    //    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size()));
    //    glBindVertexArray(0);
    //}
    shader->unbind();
}

bool DimensionalCurve_POT::intersectionCheck(glm::vec3 v)
{
    return false;
}
