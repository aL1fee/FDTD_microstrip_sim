#include "Ribbon_POT.h"

// ribbon feet are always planar
void Ribbon_POT::buildVertices()
{
    vertices->clear();
    vertices->pushToExistingArray(glm::vec3(0.0f));


    if (secondPointSel)
    {
        feetVertices->clear();
        curveVertices->clear();
        edgeVertices->clear();

        glm::vec3 normalX = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 normalV = glm::normalize(secondPoint - firstPoint);

        glm::vec3 diff = secondPoint - firstPoint;
        float l = glm::length(diff);
        length = l - 2 * tail_size;
        translationVector = origin;

        // Define a fixed rotation axis (Y-axis)
        glm::vec3 rotationAxis = __y_norm_vec3;

        // Calculate the vector in the XZ plane
        glm::vec3 projectedNormalV = glm::normalize(glm::vec3(normalV.x, 0.0f, normalV.z));

        // Compute the rotation angle in the XZ plane
        rotationAngle = -90 +  glm::degrees(atan2(projectedNormalV.x, projectedNormalV.z));

        // Adjust for edge cases (optional, if needed)
        if (glm::length(diff) < EPSILON) {
            rotationAngle = 0.0f; // No rotation for zero-length vector
        }

        scalingVector = glm::vec3(1.0f, 1.0f, width / initial_width);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, translationVector);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
        modelMatrix = glm::scale(modelMatrix, scalingVector);

        inverseModelMatrix = glm::inverse(modelMatrix);
        vertices->pushToExistingArray(glm::vec3(l, 0, 0));


        generateRibbonVertices();

        //updateMaxXYZValues();
    }

}

void Ribbon_POT::generateRibbonVertices()
{
    if (edgesOn)
    {
        buildEdges();
    }

    int numSegments = RIBBON_NUM_SEGMENTS;
    float l = glm::length(firstPoint - secondPoint) - tail_size * 2;
    float delta = l / (numSegments - 1);
    //glm::vec3 normDir = glm::normalize(glm::vec3(secondPoint.x - firstPoint.x, 0.0f, secondPoint.z - firstPoint.z));
    glm::vec3 normDir = glm::vec3(1.0f, 0.0f, 0.0f);

    for (int j = 0; j < 2; j++)
    {
        glm::vec3 pos = normDir * tail_size;
        curveVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float distanceFromCenter = (t - 0.5f) * l;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, l / 6.5f);
            glm::vec3 perpDir = glm::normalize(glm::vec3(-normDir.z, 0.0f, normDir.x));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width,
                + gaussianHeight + j * thickness, pos.z + perpDir.z * initial_width));
            curveVertices->pushToExistingArray(glm::vec3(pos.x - perpDir.x * initial_width,
                + gaussianHeight + j * thickness, pos.z));
            pos += normDir * delta;
        }
    }

    for (int j = 0; j < 2; j++)
    {
        glm::vec3 pos = normDir * tail_size;
        int k = (j == 0) ? 0 : 1;
        curveVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float distanceFromCenter = (t - 0.5f) * l;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, l / 6.5f);
            glm::vec3 perpDir = glm::normalize(glm::vec3(-normDir.z, 0.0f, normDir.x));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width * k,
                + gaussianHeight, pos.z + perpDir.z * initial_width * k));
            curveVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width * k,
                + gaussianHeight + thickness, pos.z + perpDir.z * initial_width * k));
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
    glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 normalVec = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 localOrigin = glm::vec3(0.0f);

    feetVertices->pushToExistingArray(localOrigin);
    feetVertices->pushToExistingArray(localOrigin + normalVec * initial_width);
    feetVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + normalVec * initial_width);
    feetVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f));

    feetVertices->pushToExistingArray(localOrigin + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin + normalVec * initial_width + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + normalVec * initial_width + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f) + tail_size * dir);

    std::vector<unsigned int>* inds = new std::vector<unsigned int>();
    inds->insert(inds->end(), {0,3,1,2,5,6,4,7,0,3,0,1,4,5,6,6,7,2,3});

    feetVertices->pushIndices(inds);

    feetVertices->allocateNewArray();

    localOrigin = glm::vec3(glm::length(secondPoint - firstPoint) 
        - tail_size, 0.0f, 0.0f);

    feetVertices->pushToExistingArray(localOrigin);
    feetVertices->pushToExistingArray(localOrigin + normalVec * initial_width);
    feetVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + normalVec * initial_width);
    feetVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f));

    feetVertices->pushToExistingArray(localOrigin + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin + normalVec * initial_width + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + normalVec * initial_width + tail_size * dir);
    feetVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f) + tail_size * dir);

    std::vector<unsigned int>* inds2 = new std::vector<unsigned int>();
    inds2->insert(inds2->end(), { 0,3,1,2,5,6,4,7,0,3,0,1,4,5,6,6,7,2,3 });
    feetVertices->pushIndices(inds2);

}

void Ribbon_POT::updateMaxXYZValues()
{
    minY = firstPoint.y;
    maxY = firstPoint.y + thickness + curve_height;

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

void Ribbon_POT::setupModelMatrix()
{



    inverseModelMatrix = glm::inverse(modelMatrix);

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
        

        for (int i = 0; i < feetVertices->getSize(); i++)
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
    glm::vec3 localOrigin = glm::vec3(0.0f);

    edgeVertices->pushToExistingArray(localOrigin);
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width);
    edgeVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width);
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f));

    edgeVertices->pushToExistingArray(localOrigin + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f) + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));

    std::vector<unsigned int>* inds = new std::vector<unsigned int>();
    inds->insert(inds->end(), { 0,1,2,3,0,4,7,3,2,6,5,1,5,4,7,6,5 });
    edgeVertices->pushIndices(inds);


    edgeVertices->allocateNewArray();

    localOrigin = glm::vec3(glm::length(secondPoint - firstPoint) - tail_size, 0.0f, 0.0f);

    edgeVertices->pushToExistingArray(localOrigin);
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width);
    edgeVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width);
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f));

    edgeVertices->pushToExistingArray(localOrigin + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin +
        glm::vec3(0.0f, thickness, 0.0f) + glm::vec3(0.0f, 0.0f, 1.0f) * initial_width + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));
    edgeVertices->pushToExistingArray(localOrigin + glm::vec3(0.0f, thickness, 0.0f) + tail_size * glm::vec3(1.0f, 0.0f, 0.0f));

    std::vector<unsigned int>* inds2 = new std::vector<unsigned int>();
    inds2->insert(inds2->end(), { 0,1,2,3,0,4,7,3,2,6,5,1,5,4,7,6,5 });
    edgeVertices->pushIndices(inds2);


    int numSegments = RIBBON_NUM_SEGMENTS;
    float length = glm::length(firstPoint - secondPoint) - tail_size * 2;
    float delta = length / (numSegments - 1);
    glm::vec3 normDir = glm::vec3(1.0f, 0.0f, 0.0f);

    for (int j = 0; j < 4; j++)
    {
        glm::vec3 pos = normDir * tail_size;
        edgeVertices->allocateNewArray();
        for (int i = 0; i < numSegments; ++i) {
            float t = static_cast<float>(i) / (numSegments - 1);
            float distanceFromCenter = (t - 0.5f) * length;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, length / 6.5f);
            glm::vec3 perpDir = glm::vec3(0.0f, 0.0f, 1.0f);
            switch (j)
            {
                case 0:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width,
                    gaussianHeight, pos.z + perpDir.z * initial_width));
                break;
                case 1:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width,
                    gaussianHeight + thickness, pos.z + perpDir.z * initial_width));
                break;
               case 2:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width,
                    gaussianHeight, pos.z));
                break;
                case 3:
                edgeVertices->pushToExistingArray(glm::vec3(pos.x + perpDir.x * initial_width,
                    gaussianHeight + thickness, pos.z));
                break;
            }
            pos += normDir * delta;
        }
    }
}

void Ribbon_POT::rebuild()
{
    //!
    //do it from data, not the first 2 init points, like build() does it
    build();
}

void Ribbon_POT::build()
{
    buildVertices();
    buildVAOs();
    rebuiltExpected = false;
}

void Ribbon_POT::generateModelMatrix()
{

    translationVector = origin;


    scalingVector = glm::vec3(1.0f, 1.0f, width / initial_width);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translationVector);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, scalingVector);

    //glm::vec4 point1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //glm::vec4 point2 = glm::vec4(2, 0.0f, initial_width, 1.0f);

    //firstPoint = modelMatrix * point1;
    //secondPoint = modelMatrix * point2;

    
    inverseModelMatrix = glm::inverse(modelMatrix);

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
        if (modelMatrixRegenExpected)
        {
            generateModelMatrix();
            modelMatrixRegenExpected = false;
        }
        else if (modelVertsRegenExpected)
        {
            rebuild();
            generateModelMatrix();
            modelVertsRegenExpected = false;
        }
        rebuiltExpected = false;
    }
    
    
    shader->bind();

    //setupModelMatrix();

    updateModelMatrix();


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

// boundary conditions inaccurately computed.
// the error increases as curve_height > .15f
// in order to fix it, one has to redesign the way the ribbon is constructed
// the thickness vector should be orthogonal to the segment direction vector
bool Ribbon_POT::intersectionCheck(glm::vec3 v)
{
    glm::vec4 vLocalFrame = inverseModelMatrix * glm::vec4(v, 1.0f);
    if (vLocalFrame.x > 0.0f && vLocalFrame.x < length + 2 * tail_size &&
        vLocalFrame.y > 0.0f && vLocalFrame.y < thickness + curve_height &&
        vLocalFrame.z > 0.0f && vLocalFrame.z < initial_width)
    {
        if (vLocalFrame.x < tail_size && vLocalFrame.y < thickness)
        {
            return true;
        }
        if (vLocalFrame.x > length + tail_size && 
            vLocalFrame.x < length + tail_size * 2 &&
            vLocalFrame.y < thickness)
        {
            return true;
        }

        int numSegments = RIBBON_BOUNDARY_CHECK_FEET_NUM;
        float l = glm::length(firstPoint - secondPoint) - tail_size * 2;
        float delta = l / (numSegments - 1);
        glm::vec3 normDir = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 pos = normDir * tail_size;
        for (int i = 0; i < numSegments; ++i) 
        {
            float t = static_cast<float>(i) / (numSegments - 1);
            float distanceFromCenter = (t - 0.5f) * l;
            float gaussianHeight = gaussian(distanceFromCenter, curve_height, 0.0f, l / 6.5f);
            glm::vec3 perpDir = glm::vec3(0.0f, 0.0f, 1.0f);

            if (vLocalFrame.x > pos.x + perpDir.x * initial_width &&
                vLocalFrame.x < normDir.x * delta + pos.x + perpDir.x * initial_width &&
                vLocalFrame.y > gaussianHeight &&
                vLocalFrame.y < gaussianHeight + thickness)
            {
                return true;
            }
            pos += normDir * delta;
        }
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

glm::vec3 Ribbon_POT::getCenterLocation() const
{
    glm::vec4 ret = modelMatrix * glm::vec4((length + tail_size * 2) / 2.0f,
        (thickness + curve_height) / 2.0f, initial_width / 2.0f, 1.0f);
    return glm::vec3(ret);
}

void Ribbon_POT::setScaleH(float h)
{
    curve_height = h;
    height = curve_height;
    updateFirstTwoPoints();
    rebuiltExpected = true;
    modelVertsRegenExpected = true;
}

void Ribbon_POT::setScaleW(float w)
{
    width = w;
}

void Ribbon_POT::setScaleL(float l)
{
    length = l;
    updateFirstTwoPoints();
    rebuiltExpected = true;
    modelVertsRegenExpected = true;
}

void Ribbon_POT::updateFirstTwoPoints()
{
    firstPoint = origin;
    //float l = glm::length(secondPoint - firstPoint);
    glm::vec4 point2 = glm::vec4(length + 2 * tail_size, 0.0f, 0.0, 1.0f);
    secondPoint = modelMatrix * point2;

}


void Ribbon_POT::doBeforeGUIPropertyChange()
{
    updateFirstTwoPoints();
}