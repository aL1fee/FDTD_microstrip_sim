#include "Ribbon_POT.h"

// ribbon feet are always planar
void Ribbon_POT::buildVertices()
{
    vertices->clear();
    vertices->pushToExistingArray(firstPoint);
    if (secondPointSel)
    {
        vertices->pushToExistingArray(secondPoint);
    }

    if (secondPointSel)
    {
        feetVertices->clear();
        curveVertices->clear();
        edgeVertices->clear();

        glm::vec3 dir = secondPoint - firstPoint;
        dir = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
        glm::vec3 normalVec = glm::normalize(glm::cross(dir,
            glm::vec3(0.0f, 1.0f, 0.0f)));
        origin = firstPoint - normalVec * width / 2.0f;

        edgeVertices->pushToExistingArray(origin);
        edgeVertices->pushToExistingArray(origin + normalVec * width);
        edgeVertices->pushToExistingArray(origin +
            glm::vec3(0.0f, height, 0.0f) + normalVec * width);
        edgeVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f));
        
        edgeVertices->pushToExistingArray(origin + tail_size * dir);
        edgeVertices->pushToExistingArray(origin + normalVec * width + tail_size * dir);
        edgeVertices->pushToExistingArray(origin +
            glm::vec3(0.0f, height, 0.0f) + normalVec * width + tail_size * dir);
        edgeVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f) + tail_size * dir);

        std::vector<unsigned int>* inds = new std::vector<unsigned int>();
        inds->insert(inds->end(), {0,1,2,3,0,4,7,3,2,6,5,1,5,4,7,6,5});
        edgeVertices->pushIndices(inds);

        edgeVertices->allocateNewArray();


        origin = secondPoint - normalVec * width / 2.0f;

        edgeVertices->pushToExistingArray(origin);
        edgeVertices->pushToExistingArray(origin + normalVec * width);
        edgeVertices->pushToExistingArray(origin +
            glm::vec3(0.0f, height, 0.0f) + normalVec * width);
        edgeVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f));

        edgeVertices->pushToExistingArray(origin + tail_size * dir);
        edgeVertices->pushToExistingArray(origin + normalVec * width + tail_size * dir);
        edgeVertices->pushToExistingArray(origin +
            glm::vec3(0.0f, height, 0.0f) + normalVec * width + tail_size * dir);
        edgeVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f) + tail_size * dir);

        std::vector<unsigned int>* inds2 = new std::vector<unsigned int>();
        inds2->insert(inds2->end(), { 0,1,2,3,0,4,7,3,2,6,5,1,5,4,7,6,5 });
        edgeVertices->pushIndices(inds2);

        generateRibbonVertices();
    }

}

void Ribbon_POT::generateRibbonVertices()
{
    int numSegments = 100; // Number of segments for smoothness
    float length = glm::length(firstPoint - secondPoint) - tail_size;  // Total length of the curve
    float delta = length / (numSegments - 1);  // Step size

    glm::vec3 normDir = glm::normalize(glm::vec3(secondPoint.x - firstPoint.x, 0.0f, secondPoint.z - firstPoint.z));

    glm::vec3 pos = firstPoint + normDir * tail_size; // Start from the first point

    for (int i = 0; i < numSegments; ++i) {
        // Normalized position along the curve (0 to 1)
        float t = static_cast<float>(i) / (numSegments - 1);

        // Linear interpolation for height between the two ends
        float interpolatedHeight = firstPoint.y + t * (secondPoint.y - firstPoint.y);

        // The Gaussian should be centered along the length of the curve, so we scale t from -0.5 to 0.5
        float distanceFromCenter = (t - 0.5f) * length;  // Normalized distance from the center of the curve

        // Apply the Gaussian function with appropriate parameters for the peak
        float gaussianHeight = gaussian(distanceFromCenter, .1f, 0.0f, length / 6.0f);  // Adjust the width (sigma) of the Gaussian

        // Create the two vertices for the width of the ribbon (left and right edges)
        curveVertices->pushToExistingArray(glm::vec3(pos.x, interpolatedHeight + gaussianHeight, pos.z - width / 2)); // Left edge
        curveVertices->pushToExistingArray(glm::vec3(pos.x, interpolatedHeight + gaussianHeight, pos.z + width / 2)); // Right edge

        pos += normDir * delta;
    }

}

float Ribbon_POT::gaussian(float x, float A, float mu, float sigma)
{
    return A * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
}

void Ribbon_POT::rebuildVertices()
{




}

void Ribbon_POT::buildVAOs()
{
    if (secondPointSel)
    {
        feetVAOs->clear();
        curveVAOs->clear();
        edgeVAOs->clear();
        VAOs->clear();
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->at(0)->size(), vertices->at(0)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        VAOs->push(VAO);
        VAOs->addVBO(VBO);

        /*for (int i = 0; i < edgeVertices->getSize(); i++)
        {
            unsigned int VAO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * edgeVertices->at(i)->size(), edgeVertices->at(i)->data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            edgeVAOs->push(VAO);
            edgeVAOs->addVBO(VBO);
        }*/



        for (int i = 0; i < edgeVertices->getSize(); i++)
        {
            unsigned int VAO, VBO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * edgeVertices->at(i)->size(),
                edgeVertices->at(i)->data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * edgeVertices->indicesAt(i)->size(),
                edgeVertices->indicesAt(i)->data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);            
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            edgeVAOs->push(VAO);
            edgeVAOs->addVBO(VBO);
            edgeVAOs->addEBO(EBO);
        }

        for (int i = 0; i < curveVertices->getSize(); i++)
        {
            unsigned int VAO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            unsigned int VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * curveVertices->at(i)->size(), curveVertices->at(i)->data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            curveVAOs->push(VAO);
            curveVAOs->addVBO(VBO);
        }

    }
}

void Ribbon_POT::buildEdges() {}

void Ribbon_POT::rebuild()
{
    rebuildVertices();
    buildVAOs();
}

void Ribbon_POT::build()
{
    buildVertices();
    buildVAOs();
}

void Ribbon_POT::draw()
{
    
    if (beingDrawn)
    {
        build();
        rebuiltExpected = false;
    }
    if (rebuiltExpected)
    {
        rebuild();
        rebuiltExpected = false;
    }
    
    shader->bind();
    shader->setUniform3f("color", color.x, color.y, color.z);

    if (secondPointSel)
    {
        glBindVertexArray(VAOs->at(0));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(0)->size()));
        glBindVertexArray(0);
    }

    shader->setUniform3f("color", 0.0f, 0.0f, 0.0f);

    if (edgesOn)
    {
        for (int i = 0; i < edgeVAOs->getSize(); i++) {
            glBindVertexArray(edgeVAOs->at(i));
            glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(edgeVertices->indicesAt(i)->size()),
                GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(edgeVertices->at(i)->size()));
            glBindVertexArray(0);
        }
    }

    for (int i = 0; i < curveVAOs->getSize(); i++) {
        glBindVertexArray(curveVAOs->at(i));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(curveVertices->at(i)->size()));
        glBindVertexArray(0);
    }

    shader->unbind();
}

bool Ribbon_POT::intersectionCheck(glm::vec3 v)
{
    return false;
}

void Ribbon_POT::setFirstPoint(glm::vec3 v)
{
    firstPoint = v;
    firstPointSel = true;
}

void Ribbon_POT::setSecondPotentialPoint(glm::vec3 v)
{
    secondPoint = v;
}
