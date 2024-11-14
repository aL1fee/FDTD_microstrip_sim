#include "Ribbon_POT.h"

// ribbon feet are always planar
void Ribbon_POT::buildVertices()
{
    vertices->clear();
    //vertices->pushToExistingArray(firstPoint);
    if (secondPointSel)
    {
        //vertices->pushToExistingArray(secondPoint);
    }

    if (secondPointSel)
    {
        feetVertices->clear();
        curveVertices->clear();
        edgeVertices->clear();

        generateRibbonVertices();

        // after length is updated:
        updateMaxXYZValues();
    }

}

void Ribbon_POT::generateRibbonVertices()
{
    if (edgesOn)
    {
        buildEdges();
    }


    int numSegments = RIBBON_NUM_SEGMENTS;
    length = glm::length(firstPoint - secondPoint) - tail_size;
    float delta = length / (numSegments - 1);
    glm::vec3 normDir = glm::normalize(glm::vec3(secondPoint.x - firstPoint.x, 0.0f, secondPoint.z - firstPoint.z));

    for (int j = 0; j < 2; j++)
    {
        glm::vec3 pos = firstPoint + normDir * tail_size;
        curveVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float interpolatedHeight = firstPoint.y + t * (secondPoint.y - firstPoint.y);
            float distanceFromCenter = (t - 0.5f) * length;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, length / 8.0f);
            glm::vec3 perpDir = glm::normalize(glm::vec3(-normDir.z, 0.0f, normDir.x));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * width / 2,
                interpolatedHeight + gaussianHeight + j * height, pos.z + perpDir.z * width / 2));
            curveVertices->pushToExistingArray(glm::vec3(pos.x - perpDir.x * width / 2,
                interpolatedHeight + gaussianHeight + j * height, pos.z - perpDir.z * width / 2));
            pos += normDir * delta;
        }
    }

    for (int j = 0; j < 2; j++)
    {
        glm::vec3 pos = firstPoint + normDir * tail_size;
        int k = (j == 0) ? -1 : 1;
        curveVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float interpolatedHeight = firstPoint.y + t * (secondPoint.y - firstPoint.y);
            float distanceFromCenter = (t - 0.5f) * length;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, length / 8.0f);
            glm::vec3 perpDir = glm::normalize(glm::vec3(-normDir.z, 0.0f, normDir.x));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * width / 2 * k,
                interpolatedHeight + gaussianHeight, pos.z + perpDir.z * width / 2 * k));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * width / 2 * k,
                interpolatedHeight + gaussianHeight + height, pos.z + perpDir.z * width / 2 * k));
            pos += normDir * delta;
        }
    }
    
    buildFeet();

}

float Ribbon_POT::gaussian(float x, float A, float mu, float sigma)
{
    return A * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
}

void Ribbon_POT::buildFeet()
{
    glm::vec3 dir = secondPoint - firstPoint;
    dir = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
    glm::vec3 normalVec = glm::normalize(glm::cross(dir,
        glm::vec3(0.0f, 1.0f, 0.0f)));
    origin = firstPoint - normalVec * width / 2.0f;

    feetVertices->pushToExistingArray(origin);
    feetVertices->pushToExistingArray(origin + normalVec * width);
    feetVertices->pushToExistingArray(origin +
        glm::vec3(0.0f, height, 0.0f) + normalVec * width);
    feetVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f));

    feetVertices->pushToExistingArray(origin + tail_size * dir);
    feetVertices->pushToExistingArray(origin + normalVec * width + tail_size * dir);
    feetVertices->pushToExistingArray(origin +
        glm::vec3(0.0f, height, 0.0f) + normalVec * width + tail_size * dir);
    feetVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f) + tail_size * dir);

    std::vector<unsigned int>* inds = new std::vector<unsigned int>();
    inds->insert(inds->end(), {0,3,1,2,5,6,4,7,0,3,0,1,4,5,6,6,7,2,3});

    feetVertices->pushIndices(inds);

    feetVertices->allocateNewArray();

    origin = secondPoint - normalVec * width / 2.0f;

    feetVertices->pushToExistingArray(origin);
    feetVertices->pushToExistingArray(origin + normalVec * width);
    feetVertices->pushToExistingArray(origin +
        glm::vec3(0.0f, height, 0.0f) + normalVec * width);
    feetVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f));

    feetVertices->pushToExistingArray(origin + tail_size * dir);
    feetVertices->pushToExistingArray(origin + normalVec * width + tail_size * dir);
    feetVertices->pushToExistingArray(origin +
        glm::vec3(0.0f, height, 0.0f) + normalVec * width + tail_size * dir);
    feetVertices->pushToExistingArray(origin + glm::vec3(0.0f, height, 0.0f) + tail_size * dir);

    std::vector<unsigned int>* inds2 = new std::vector<unsigned int>();
    inds2->insert(inds2->end(), { 0,3,1,2,5,6,4,7,0,3,0,1,4,5,6,6,7,2,3 });
    feetVertices->pushIndices(inds2);

}

void Ribbon_POT::updateMaxXYZValues()
{
    minY = firstPoint.y;
    maxY = firstPoint.y + height + curve_height;

    glm::vec3 dir = secondPoint - firstPoint;
    dir = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
    glm::vec3 normalVec = glm::normalize(glm::cross(dir,
        glm::vec3(0.0f, 1.0f, 0.0f)));
    origin = firstPoint - normalVec * width / 2.0f;

    glm::vec3 extreme1 = origin;
    glm::vec3 extreme2 = origin + width * normalVec;
    glm::vec3 extreme3 = origin + (2 * tail_size + length) * dir;
    glm::vec3 extreme4 = origin + (2 * tail_size + length) * dir + width * normalVec;

    minZ = min(extreme1.z, min(extreme2.z, min(extreme3.z, extreme4.z)));
    minX = min(extreme1.x, min(extreme2.x, min(extreme3.x, extreme4.x)));
    maxZ = max(extreme1.z, max(extreme2.z, max(extreme3.z, extreme4.z)));
    maxX = max(extreme1.x, max(extreme2.x, max(extreme3.x, extreme4.x)));

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
        //unsigned int VAO;
        //glGenVertexArrays(1, &VAO);
        //glBindVertexArray(VAO);
        //unsigned int VBO;
        //glGenBuffers(1, &VBO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices->at(0)->size(), vertices->at(0)->data(), GL_STATIC_DRAW);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);
        //VAOs->push(VAO);
        //VAOs->addVBO(VBO);

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



        for (int i = 0; i < 2; i++)
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

        for (int i = 2; i < edgeVertices->getSize(); i++)
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

        for (int i = 0; i < 2; i++)
        {
            unsigned int VAO, VBO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * feetVertices->at(i)->size(),
                feetVertices->at(i)->data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * feetVertices->indicesAt(i)->size(),
                feetVertices->indicesAt(i)->data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            feetVAOs->push(VAO);
            feetVAOs->addVBO(VBO);
            feetVAOs->addEBO(EBO);
        }

    }
}

void Ribbon_POT::buildEdges() 
{
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
    inds->insert(inds->end(), { 0,1,2,3,0,4,7,3,2,6,5,1,5,4,7,6,5 });
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


    int numSegments = RIBBON_NUM_SEGMENTS;
    float length = glm::length(firstPoint - secondPoint) - tail_size;
    float delta = length / (numSegments - 1);
    glm::vec3 normDir = glm::normalize(glm::vec3(secondPoint.x - firstPoint.x, 0.0f, secondPoint.z - firstPoint.z));

    for (int j = 0; j < 4; j++)
    {
        glm::vec3 pos = firstPoint + normDir * tail_size;
        edgeVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float interpolatedHeight = firstPoint.y + t * (secondPoint.y - firstPoint.y);
            float distanceFromCenter = (t - 0.5f) * length;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, length / 8.0f);
            glm::vec3 perpDir = glm::normalize(glm::vec3(-normDir.z, 0.0f, normDir.x));
            switch (j)
            {
                case 0:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * width / 2,
                    interpolatedHeight + gaussianHeight, pos.z + perpDir.z * width / 2));
                break;
                case 1:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * width / 2,
                    interpolatedHeight + gaussianHeight + height, pos.z + perpDir.z * width / 2));
                break;
                case 2:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x - perpDir.x * width / 2,
                    interpolatedHeight + gaussianHeight, pos.z - perpDir.z * width / 2));
                break;
                case 3:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x - perpDir.x * width / 2,
                    interpolatedHeight + gaussianHeight + height, pos.z - perpDir.z * width / 2));
                break;
            }
            pos += normDir * delta;
        }
    }
}

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
    //shader->setUniform3f("color", color.x, color.y, color.z);

    //if (secondPointSel)
    //{
    //    glBindVertexArray(VAOs->at(0));
    //    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(0)->size()));
    //    glBindVertexArray(0);
    //}

    shader->setUniform3f("color", 0.0f, 0.0f, 0.0f);

    if (edgesOn)
    {
        if (edgeVAOs->getSize() != 0)
        {
            for (int i = 0; i < 2; i++) {
                glBindVertexArray(edgeVAOs->at(i));
                glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(edgeVertices->indicesAt(i)->size()),
                    GL_UNSIGNED_INT, 0);
                //glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(edgeVertices->at(i)->size()));
                glBindVertexArray(0);
            }
            for (int i = 2; i < edgeVAOs->getSize(); i++) {
                glBindVertexArray(edgeVAOs->at(i));
                glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(edgeVertices->at(i)->size()));
                glBindVertexArray(0);
            }
        }
    }

    shader->setUniform3f("color", color.x, color.y, color.z);


    for (int i = 0; i < curveVAOs->getSize(); i++) {
        glBindVertexArray(curveVAOs->at(i));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(curveVertices->at(i)->size()));
        glBindVertexArray(0);
    }

    for (int i = 0; i < feetVAOs->getSize(); i++) {
        glBindVertexArray(feetVAOs->at(i));
        glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(feetVertices->indicesAt(i)->size()),
            GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    shader->unbind();
}

bool Ribbon_POT::intersectionCheck(glm::vec3 v)
{
    float max_X = MAX_COORD_VALUE;
    float min_X = MIN_COORD_VALUE;
    float max_Y = MAX_COORD_VALUE;
    float min_Y = MIN_COORD_VALUE;
    float max_Z = MAX_COORD_VALUE;
    float min_Z = MIN_COORD_VALUE;
    if (v.x < maxX && v.x > minX && v.y < maxY && v.y > minY && v.z < maxZ && v.z > minZ)
    {
        glm::vec3 dir = secondPoint - firstPoint;
        dir = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
        glm::vec3 normalVec = glm::normalize(glm::cross(dir,
            glm::vec3(0.0f, 1.0f, 0.0f)));
        origin = firstPoint - normalVec * width / 2.0f;
        //for (int i = 1; i <= RIBBON_BOUNDARY_CHECK_FEET_NUM; i++)
        //{
        //    glm::vec3 extreme1 = origin + (tail_size * (i - 1) /
        //        static_cast<float>(RIBBON_BOUNDARY_CHECK_FEET_NUM)) * dir;
        //    glm::vec3 extreme2 = origin + (tail_size * (i - 1) /
        //        static_cast<float>(RIBBON_BOUNDARY_CHECK_FEET_NUM)) * dir + width * normalVec;
        //    glm::vec3 extreme3 = origin + (tail_size * i /
        //        static_cast<float>(RIBBON_BOUNDARY_CHECK_FEET_NUM)) * dir;
        //    glm::vec3 extreme4 = origin + (tail_size * i /
        //        static_cast<float>(RIBBON_BOUNDARY_CHECK_FEET_NUM)) * dir + width * normalVec;

        //    min_Z = min(extreme1.z, min(extreme2.z, min(extreme3.z, extreme4.z)));
        //    min_X = min(extreme1.x, min(extreme2.x, min(extreme3.x, extreme4.x)));
        //    max_Z = max(extreme1.z, max(extreme2.z, max(extreme3.z, extreme4.z)));
        //    max_X = max(extreme1.x, max(extreme2.x, max(extreme3.x, extreme4.x)));

        //    if (v.z > min_Z && v.z < max_Z && v.x > min_X && v.x < max_X)
        //    {
        //        std::cout << "Ribbon detected!" << std::endl;
        //        return true;
        //    }

        //}




    }
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
