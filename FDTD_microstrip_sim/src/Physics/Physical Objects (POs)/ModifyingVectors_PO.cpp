#include "ModifyingVectors_PO.h"

ModifyingVectors_PO::ModifyingVectors_PO(float s, ModyfingVectorType t,
    glm::vec3 c, Shader* sh)
{
    size = s;
    type = t;
    shader = sh;
    origin = c;
    colorX = glm::vec3(1.0f, 0.0f, 0.0f);
    colorY = glm::vec3(0.0f, 1.0f, 0.0f);
    colorZ = glm::vec3(0.0f, 0.0f, 1.0f);
    arrowAngle = glm::radians(MODIFYING_VECTORS_ARROW_ANGLE);
}

void ModifyingVectors_PO::buildVertices()
{
    vertices->pushToExistingArray(origin);
    vertices->pushToExistingArray(colorX);
    vertices->pushToExistingArray(origin + glm::vec3(size, .0f, .0f));
    vertices->pushToExistingArray(colorX);
    vertices->allocateNewArray();
    vertices->pushToExistingArray(origin);
    vertices->pushToExistingArray(colorY);
    vertices->pushToExistingArray(origin + glm::vec3(.0f, size, .0f));
    vertices->pushToExistingArray(colorY);
    vertices->allocateNewArray();
    vertices->pushToExistingArray(origin);
    vertices->pushToExistingArray(colorZ);
    vertices->pushToExistingArray(origin + glm::vec3(.0f, .0f, size));
    vertices->pushToExistingArray(colorZ);
    switch (type) {
        case Translation:
            constructTranslationArrows();
            break;
        case Scaling:
            constructScalingArrows();
            break;
        case Rotation:
            break;
    }
}

void ModifyingVectors_PO::buildVAOs()
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

void ModifyingVectors_PO::buildEdges() {}

void ModifyingVectors_PO::constructTranslationArrows()
{
    float radius = MODIFYING_VECTORS_ARROW_LENGTH
        * sin(arrowAngle);
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 col = glm::vec3(0.0f);
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 3; i++) 
    {
        switch (i) {
            case 0:
                x = 1;
                col = colorX;
                break;
            case 1:
                x = 0;
                y = 1;
                col = colorY;
                break;
            case 2:
                y = 0;
                z = 1;
                col = colorZ;
                break;
        }
        point = origin + glm::vec3(x * size, y * size, z * size);
        vertices->allocateNewArray();
        vertices->pushToExistingArray(point);
        vertices->pushToExistingArray(col);
        for (float j = 0.0f; j < 450.0f; j += MODIFYING_VECTORS_ARROW_CIRCLE_INTERVAL) {
            float angle = glm::radians(j);
            glm::vec3 offset = glm::vec3(MODIFYING_VECTORS_ARROW_LENGTH * cos(arrowAngle),
                radius * sin(angle), radius * cos(angle));
            if (i == 0) {
                vertices->pushToExistingArray(point - offset);
                vertices->pushToExistingArray(col);
            }
            else if (i == 1) {
                offset = glm::vec3(radius * cos(angle), 
                    MODIFYING_VECTORS_ARROW_LENGTH * cos(arrowAngle), 
                    radius * sin(angle));
                vertices->pushToExistingArray(point - offset);
                vertices->pushToExistingArray(col);
            }
            else if (i == 2) {
                offset = glm::vec3(radius * sin(angle), radius * cos(angle),
                    MODIFYING_VECTORS_ARROW_LENGTH * cos(arrowAngle));
                vertices->pushToExistingArray(point - offset);
                vertices->pushToExistingArray(col);
            }
        }
    }
}

//nasty
void ModifyingVectors_PO::constructScalingArrows()
{
    float radius = MODIFYING_VECTORS_ARROW_LENGTH
        * sin(arrowAngle);
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 col = glm::vec3(0.0f);
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 3; i++)
    {
        switch (i) {
        case 0:
            x = 1;
            col = colorX;
            break;
        case 1:
            x = 0;
            y = 1;
            col = colorY;
            break;
        case 2:
            y = 0;
            z = 1;
            col = colorZ;
            break;
        }
        point = origin + glm::vec3(x * size, y * size, z * size);
        int xInner = 0, yInner = 0, zInner = 0;
        if (i == 0)
        {
            point += glm::vec3(-MODIFYING_VECTORS_ARROW_LENGTH, -radius / 2.0f, -radius / 2.0f);
            vertices->allocateNewArray();
            for (int j = 0; j < 5; j++) {
                zInner = (j == 1 || j == 2) ? 1 : 0;
                yInner = (j == 2 || j == 3) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(0, radius * yInner, radius * zInner));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(MODIFYING_VECTORS_ARROW_LENGTH, radius * yInner, radius * zInner));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                yInner = (j == 1) ? j : 0;
                vertices->pushToExistingArray(point + glm::vec3(0, radius * yInner, 0));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(0, radius * yInner, radius));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                yInner = (j == 1) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(MODIFYING_VECTORS_ARROW_LENGTH, radius * yInner, 0));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(MODIFYING_VECTORS_ARROW_LENGTH, radius * yInner, radius));
                addColorVertex(col);
            }
        }
        else if (i == 1)
        {
            point += glm::vec3(-radius / 2.0f, -MODIFYING_VECTORS_ARROW_LENGTH, -radius / 2.0f);
            vertices->allocateNewArray();
            for (int j = 0; j < 5; j++) {
                xInner = (j == 1 || j == 2) ? 1 : 0;
                zInner = (j == 2 || j == 3) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, 0.0f, radius * zInner));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, MODIFYING_VECTORS_ARROW_LENGTH, radius * zInner));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                xInner = (j == 1) ? j : 0;
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, 0.0f, 0.0f));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, 0.0f, radius));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                xInner = (j == 1) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, MODIFYING_VECTORS_ARROW_LENGTH, 0.0f));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, MODIFYING_VECTORS_ARROW_LENGTH, radius));
                addColorVertex(col);
            }
        }
        else if (i == 2)
        {
            point += glm::vec3(-radius / 2.0f, -radius / 2.0f, -MODIFYING_VECTORS_ARROW_LENGTH);
            vertices->allocateNewArray();
            for (int j = 0; j < 5; j++) {
                xInner = (j == 1 || j == 2) ? 1 : 0;
                yInner = (j == 2 || j == 3) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, radius * yInner, 0.0f));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius * xInner, radius * yInner, MODIFYING_VECTORS_ARROW_LENGTH));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                yInner = (j == 1) ? j : 0;
                vertices->pushToExistingArray(point + glm::vec3(0.0f, radius * yInner, 0.0f));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius, radius * yInner, 0.0f));
                addColorVertex(col);
            }
            vertices->allocateNewArray();
            for (int j = 0; j < 2; j++) {
                yInner = (j == 1) ? 1 : 0;
                vertices->pushToExistingArray(point + glm::vec3(0.0f, radius * yInner, MODIFYING_VECTORS_ARROW_LENGTH));
                addColorVertex(col);
                vertices->pushToExistingArray(point + glm::vec3(radius, radius * yInner, MODIFYING_VECTORS_ARROW_LENGTH));
                addColorVertex(col);
            }
        }
    }
}

void ModifyingVectors_PO::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void ModifyingVectors_PO::rebuild()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
}

void ModifyingVectors_PO::draw()
{
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }
    glDisable(GL_DEPTH_TEST);
    shader->bind();
    switch (type) {
        case Translation:
        for (int i = 0; i < 3; i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
            glBindVertexArray(0);
        }
        for (int i = 3; i < vertices->getSize(); i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
            glBindVertexArray(0);
        }
        break;
        case Scaling:
            for (int i = 0; i < 3; i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
                glBindVertexArray(0);
            }
            for (int i = 3; i < vertices->getSize(); i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
                glBindVertexArray(0);
            }
            break;
    }

    shader->unbind();
    glEnable(GL_DEPTH_TEST);
}

bool ModifyingVectors_PO::intersectionCheck(glm::vec3 v)
{
    return false;
}

glm::vec3 ModifyingVectors_PO::intersectionDirection(glm::vec3 v)
{
    if ((origin.x < v.x && origin.x + size > v.x) &&
        (origin.y + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.y &&
            origin.y - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.y) &&
        (origin.z + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.z &&
            origin.z - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.z))
    {
        return glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if ((origin.y < v.y && origin.y + size > v.y) &&
        (origin.x + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.x &&
            origin.x - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.x) &&
        (origin.z + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.z &&
            origin.z - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.z))
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if ((origin.z < v.z && origin.z + size > v.z) &&
        (origin.y + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.y &&
            origin.y - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.y) &&
        (origin.x + MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > v.x &&
            origin.x - MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < v.x))
    {
        // neg z dir
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }
    return glm::vec3(0.0f);
}
