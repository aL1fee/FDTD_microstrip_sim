#include "CuboidArray_POT.h"

void CuboidArray_POT::buildVertices()
{
	for (int countZ = 0; countZ < numPadsZ; countZ++) 
    {
        for (int countX = 0; countX < numPadsX; countX++)
        {
            vertices->allocateNewArray();
            int x = 0, y = 0, z = 0;
            for (int i = 0; i < 5; i++) {
                z = (i == 1 || i == 2) ? 1 : 0;
                y = (i == 2 || i == 3) ? 1 : 0;
                vertices->pushToExistingArray(origin +
                    glm::vec3(countX * sizeX + countX * padSeparationX, height * y,
                        countZ * sizeZ + countZ * padSeparationZ + sizeZ * z));
                addColorVertex(color);
                vertices->pushToExistingArray(origin +
                    glm::vec3(countX * sizeX + countX * padSeparationX + sizeX, height * y,
                        countZ * sizeZ + countZ * padSeparationZ + sizeZ * z));
                addColorVertex(color);
            }
            vertices->allocateNewArray();
            for (int i = 0; i < 2; i++) {
                y = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin +
                    glm::vec3(countX * sizeX + countX * padSeparationX, height * y,
                        countZ * sizeZ + countZ * padSeparationZ));
                addColorVertex(color);
                vertices->pushToExistingArray(origin + 
                    glm::vec3(countX * sizeX + countX * padSeparationX, height * y,
                        countZ * sizeZ + countZ * padSeparationZ + sizeZ));
                addColorVertex(color);
            }
            vertices->allocateNewArray();
            for (int i = 0; i < 2; i++) {
                y = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin + 
                    glm::vec3(countX * sizeX + countX * 
                    padSeparationX + sizeX, height * y, 
                    countZ * sizeZ + countZ * padSeparationZ));
                addColorVertex(color);
                vertices->pushToExistingArray(origin + 
                    glm::vec3(countX * sizeX + countX *
                        padSeparationX + sizeX, height * y, 
                        countZ * sizeZ + countZ * padSeparationZ + sizeZ));
                addColorVertex(color);
            }
        }
	}
    if (edgesOn) {
        buildEdges();
    }
}

void CuboidArray_POT::buildVAOs()
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

void CuboidArray_POT::buildEdges()
{
    for (int countZ = 0; countZ < numPadsZ; countZ++)
    {
        for (int countX = 0; countX < numPadsX; countX++)
        {
            vertices->allocateNewArray();
            int x = 0, y = 0, z = 0;
            for (int i = 0; i < 4; i++) {
                z = (i == 1 || i == 2) ? 1 : 0;
                y = (i == 2 || i == 3) ? 1 : 0;
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX + sizeX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
            }
            for (int i = 0; i < 2; i++) {
                y = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ));
                addColorVertex(glm::vec3(.1f));
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ + sizeZ));
                addColorVertex(glm::vec3(.1f));
            }
            for (int i = 0; i < 2; i++) {
                z = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX,
                    0, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX,
                    height, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
            }
            for (int i = 0; i < 2; i++) {
                y = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX + sizeX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ));
                addColorVertex(glm::vec3(.1f));
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX + sizeX,
                    height * y, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
            }
            for (int i = 0; i < 2; i++) {
                z = (i == 1) ? 1 : 0;
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX + sizeX,
                    0, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
                vertices->pushToExistingArray(origin + glm::vec3(countX * sizeX + countX * padSeparationX + sizeX,
                    height, countZ * sizeZ + countZ * padSeparationZ + z * sizeZ));
                addColorVertex(glm::vec3(.1f));
            }
        }
    }
}

void CuboidArray_POT::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void CuboidArray_POT::draw()
{
    if (rebuiltExpected) {
        build();
        rebuiltExpected = false;
    }
    shader->bind();
    //std::cout << vertices->getSize() << std::endl;
    for (int i = 0; i < numPadsX * numPadsZ * 3; i++) {
        glBindVertexArray(VAOs->at(i));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
        glBindVertexArray(0);
    }
    if (edgesOn) {
        for (int i = numPadsX * numPadsZ * 3; i < vertices->getSize(); i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size()));
            glBindVertexArray(0);
        }
    }
    shader->unbind();
}

bool CuboidArray_POT::intersectionCheck(glm::vec3 v)
{
    for (int countZ = 0; countZ < numPadsZ; countZ++)
    {
        for (int countX = 0; countX < numPadsX; countX++)
        {
            if (origin.y > v.y || origin.y + height < v.y)
            {
                return false;
            }
            if ((origin. x + countX * sizeX + countX * padSeparationX < v.x &&
                origin.x + countX * sizeX + countX * padSeparationX + sizeX > v.x) &&
                (origin.z + countZ * sizeZ + countZ * padSeparationZ < v.z) &&
                (origin.z + countZ * sizeZ + countZ * padSeparationZ + sizeZ > v.z))
            {
                return true;
            }
        }
    }
    return false;
}

glm::vec3 CuboidArray_POT::getCenterLocation() const
{
    return origin + glm::vec3((numPadsX * sizeX + (numPadsX - 1) * padSeparationX) / 2.0f,
        height / 2.0f, (numPadsZ * sizeZ + (numPadsZ - 1) * padSeparationZ) / 2.0f);
}

void CuboidArray_POT::setScaleL(float l)
{
    float totalPadSeparation = l - sizeX * numPadsX;
    padSeparationX = totalPadSeparation / (numPadsX - 1);
}

void CuboidArray_POT::setScaleW(float w)
{
    float totalPadSeparation = w - sizeZ * numPadsZ;
    padSeparationZ = totalPadSeparation / (numPadsZ - 1);
}
