#include "DimensionalCurve_POT.h"

// process vertices to be more accurate
void DimensionalCurve_POT::buildVertices()
{
    if (processedVertices.size() == 0 && movementStarted)
    {
        processedVertices.push_back(unprocessedVertex);
        prevNormal1 = glm::vec3(1.0f, 0.0f, 0.0f);
        prevNormal2 = glm::vec3(0.0f, 0.0f, -1.0f);
        prevCenter = unprocessedVertex;
        glm::vec3 prevTangent(0.0f);
        return;
    }
    else if (movementStarted && !curveTerminated)
    {
        glm::vec3 lastUnprocessedVertex = unprocessedVertex;
        glm::vec3 lastProcessedVertex = processedVertices.at(processedVertices.size() - 1);
        float lengthSinceLastProcessedVertex =
            glm::length(lastUnprocessedVertex - lastProcessedVertex);
        if (lengthSinceLastProcessedVertex > DIMENSIONAL_CURVE_DELTA_LENGTH || lastVertexEntered)
        {
            glm::vec3 normalDirVec = glm::normalize(lastUnprocessedVertex - lastProcessedVertex);
            glm::vec3 newProcessedVec = lastProcessedVertex + DIMENSIONAL_CURVE_DELTA_LENGTH * normalDirVec;

            processedVertices.push_back(newProcessedVec);

            if (unprocessedVertex.y <= lastVertex.y)
            {
                curveTerminated = true;
                beingDrawn = false;
            }


            rebuildVertices();
        }
    }
}

void DimensionalCurve_POT::rebuildVertices()
{
    cylindersVertices->clear();
    edgesVertices->clear();
    edgesVertices->allocateNEmptyArrays(4);
    crossSecVertices->clear();
    crossSecVertices->allocateNEmptyArrays(1);
    ballFootVertices->clear();
    ballFootVertices->allocateNewEmptyArray();
    ballFootVertices->allocateNewEmptyArray();

    glm::vec3 prevCenter = processedVertices.at(0);
    glm::vec3 prevTangent(0.0f);
    glm::vec3 prevNormal1(1.0f, 0.0f, 0.0f);
    glm::vec3 prevNormal2(0.0f, 0.0f, -1.0f);

    std::cout << "diamter" << diameter << std::endl;
    for (int j = 1; j < processedVertices.size(); j++)
    {
        cylindersVertices->allocateNewArray();
        crossSecVertices->allocateNewArray();

        glm::vec3 tangent = glm::normalize(processedVertices.at(j) -
            processedVertices.at(j - 1));

        glm::vec3 normal1, normal2;
        if (j == 1) {
            normal1 = prevNormal1;
            normal2 = prevNormal2;
        }
        else 
        {
            glm::vec3 rotationAxis = glm::cross(prevTangent, tangent);
            float rotationAngle = acos(glm::dot(prevTangent, tangent));
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);

            normal1 = glm::vec3(rotationMatrix * glm::vec4(prevNormal1, 1.0f));
            normal2 = glm::vec3(rotationMatrix * glm::vec4(prevNormal2, 1.0f));
        }

        glm::vec3 center = processedVertices.at(j);

        if (j == processedVertices.size() - 1)
        {
            crossSecVertices->allocateNewEmptyArray();
        }
        for (int i = 0; i <= DIMENSIONAL_CURVE_CIRCLE_NUM_SEGMENTS; i++)
        {
            float angle = 2.0f * static_cast<float>(M_PI) * i / DIMENSIONAL_CURVE_CIRCLE_NUM_SEGMENTS;

            glm::vec3 circlePoint1 = prevCenter + diameter / 2.0f *
                (prevNormal1 * cos(angle) + prevNormal2 * sin(angle));
            glm::vec3 circlePoint2 = center + diameter / 2.0f *
                (normal1 * cos(angle) + normal2 * sin(angle));

            if (j != processedVertices.size() - 1)
            {
                cylindersVertices->pushToExistingArray(circlePoint1);
                cylindersVertices->pushToExistingArray(circlePoint2);
            }
            if (i % 4 == 0)
            {
                edgesVertices->pushToIndArray(i / 4, circlePoint1);
            }
            if (j == 1)
            {
                prevNormal1 = glm::vec3(1.0f, 0.0f, 0.0f);
                prevNormal2 = glm::vec3(0.0f, 0.0f, -1.0f);

                glm::vec3 originCrossSec = prevCenter + diameter * 1.5f *
                    (prevNormal1 * cos(angle) + prevNormal2 * sin(angle));

                ballFootVertices->pushToIndArray(0, originCrossSec);
                ballFootVertices->pushToIndArray(0, circlePoint2);

                crossSecVertices->pushToIndArray(0, originCrossSec);
                crossSecVertices->pushToIndArray(1, circlePoint2);

            }
            else if (j == processedVertices.size() - 1)
            {
                if (curveTerminated)
                {
                    glm::vec3 lastNormal1 = prevNormal1;
                    glm::vec3 lastNormal2 = prevNormal2;

                    //optional
                    lastNormal1 = glm::normalize(lastNormal1 + glm::vec3(0.01f, 0.01f, 0.0f));
                    lastNormal2 = glm::normalize(lastNormal2 + glm::vec3(0.0f, 0.01f, 0.01f));

                    glm::vec3 endCrossSec = center + diameter * 1.5f *
                        (lastNormal1 * cos(angle) + lastNormal2 * sin(angle));
                    endCrossSec.y = lastVertex.y;

                    crossSecVertices->pushToIndArray(crossSecVertices->getSize() - 2, circlePoint1);
                    crossSecVertices->pushToIndArray(crossSecVertices->getSize() - 1, endCrossSec);

                    //edgesVertices->pushToIndArray(i / 4, endCrossSec);

                    ballFootVertices->pushToIndArray(1, circlePoint1);
                    ballFootVertices->pushToIndArray(1, endCrossSec);

                    beingDrawn = false;
                }
            }
            else {
                crossSecVertices->pushToExistingArray(circlePoint2);
            }
            if (maxX < circlePoint2.x) {
                maxX = std::max(maxX, std::max(circlePoint1.x, circlePoint2.x));
            }
            if (minX > circlePoint2.x) {
                minX = std::min(minX, std::min(circlePoint1.x, circlePoint2.x));
            }
            if (maxY < circlePoint2.y) {
                maxY = std::max(maxY, std::max(circlePoint1.y, circlePoint2.y));
            }
            if (minY > circlePoint2.y) {
                minY = std::min(minY, std::min(circlePoint1.y, circlePoint2.y));
            }
            if (maxZ < circlePoint2.z) {
                maxZ = std::max(maxZ, std::max(circlePoint1.z, circlePoint2.z));
            }
            if (minZ > circlePoint2.z) {
                minZ = std::min(minZ, std::min(circlePoint1.z, circlePoint2.z));
            }
        }
        prevTangent = tangent;
        prevCenter = center;
        prevNormal1 = normal1;
        prevNormal2 = normal2;
    }
}

void DimensionalCurve_POT::buildVAOs()
{
    VAOs->clear();
    cylindersVAOs->clear();
    edgesVAOs->clear();
    crossSecVAOs->clear();
    ballFootVAOs->clear();

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
    VAOs->addVBO(VBO);

    for (int i = 0; i < cylindersVertices->getSize(); i++)
    {
        unsigned int VAO_cyl;
        glGenVertexArrays(1, &VAO_cyl);
        glBindVertexArray(VAO_cyl);
        unsigned int VBO_cyl;
        glGenBuffers(1, &VBO_cyl);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_cyl);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cylindersVertices->at(i)->size(), cylindersVertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        cylindersVAOs->push(VAO_cyl);
        cylindersVAOs->addVBO(VBO_cyl);
    }
    for (int i = 0; i < edgesVertices->getSize(); i++)
    {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * edgesVertices->at(i)->size(), edgesVertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        edgesVAOs->push(VAO);
        edgesVAOs->addVBO(VBO);
    }

    for (int i = 0; i < crossSecVertices->getSize(); i++)
    {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * crossSecVertices->at(i)->size(), crossSecVertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        crossSecVAOs->push(VAO);
        crossSecVAOs->addVBO(VBO);
    }
    for (int i = 0; i < ballFootVertices->getSize(); i++)
    {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ballFootVertices->at(i)->size(), ballFootVertices->at(i)->data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        ballFootVAOs->push(VAO);
        ballFootVAOs->addVBO(VBO);
    }
}

void DimensionalCurve_POT::buildEdges() {}

void DimensionalCurve_POT::rebuild()
{
    rebuildVertices();
    buildVAOs();
}

void DimensionalCurve_POT::addPoint(glm::vec3 v)
{
    if (processedVertices.size() == 0)
    {
        processedVertices.push_back(v);
        indexProcessed++;
    }
}

void DimensionalCurve_POT::updateUnprocessedVertex(glm::vec3 v)
{
    unprocessedVertex = v;
    movementStarted = true;
}

void DimensionalCurve_POT::build()
{
    buildVertices();
    buildVAOs();
}

void DimensionalCurve_POT::draw()
{
    if (beingDrawn)
    {
        std::cout << "being drawn" << std::endl;
        build();
        rebuiltExpected = false;
    }
    if (rebuiltExpected)
    {
        std::cout << "rebuilding" << std::endl;
        rebuild();
        rebuiltExpected = false;
    }

    shader->bind();
    shader->setUniform3f("color", color.x, color.y, color.z);

    //glBindVertexArray(VAOs->at(0));
    //glDrawArrays(GL_LINE_STRIP, 0,
    //    static_cast<GLsizei>(processedVertices.size()));
    //glBindVertexArray(0);

    for (int i = 0; i < cylindersVAOs->getSize(); i++) {
        glBindVertexArray(cylindersVAOs->at(i));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(cylindersVertices->at(i)->size()));
        glBindVertexArray(0);
    }

    shader->setUniform3f("color", 0.0f, 0.0f, 0.0f);

    if (edgesOn)
    {
        for (int i = 0; i < edgesVAOs->getSize(); i++) {
            glBindVertexArray(edgesVAOs->at(i));
            glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(edgesVertices->at(i)->size()));
            glBindVertexArray(0);
        }
    }
    if (crossSectionsOn)
    for (int i = 0; i < crossSecVAOs->getSize(); i++) {
        glBindVertexArray(crossSecVAOs->at(i));
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(crossSecVertices->at(i)->size()));
        glBindVertexArray(0);
    }
    shader->setUniform3f("color", color.x, color.y, color.z);

    for (int i = 0; i < ballFootVAOs->getSize(); i++) {
        glBindVertexArray(ballFootVAOs->at(i));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(ballFootVertices->at(i)->size()));
        glBindVertexArray(0);
    }


    shader->unbind();
}

bool DimensionalCurve_POT::intersectionCheck(glm::vec3 v)
{
    if (processedVertices.size() == 0) {
        return false;
    }
    if (v.x < maxX && v.x > minX && v.y < maxY && v.y > minY && v.z < maxZ && v.z > minZ)
    {
        for (int i = 1; i < processedVertices.size(); i++)
        {
            glm::vec3 cylSegmentPointOne = processedVertices.at(i - 1);
            glm::vec3 cylSegmentPointTwo = processedVertices.at(i);
            glm::vec3 lineSeg = cylSegmentPointTwo - cylSegmentPointOne;
            glm::vec3 vecV = v - cylSegmentPointOne;
            float t = glm::dot(lineSeg, vecV) / glm::dot(lineSeg, lineSeg);
            t = glm::clamp(t, 0.0f, 1.0f);
            glm::vec3 closestPoint = cylSegmentPointOne + t * lineSeg;
            float dist = glm::length(v - closestPoint);
            if (dist < diameter / 2.0f)
            {
                std::cout << "Wire intersected" << std::endl;
                return true;
            }
        }
    }
    return false;
}

glm::vec3 DimensionalCurve_POT::getCenterLocation() const
{
    return glm::vec3(minX + (maxX - minX) / 2.0f, minY + (maxY - minY) / 2.0f, minZ + (maxZ - minZ) / 2.0f);
}
