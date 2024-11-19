#include "ModifyingVectors_PO.h"

ModifyingVectors_PO::ModifyingVectors_PO(float s, ModyfingVectorType t,
    glm::vec3 c, Shader* sh)
{
    size = s;
    type = t;
    shader = sh;
    origin = c;
    colorX = __color_red;
    colorY = __color_green;
    colorZ = __color_blue;
    arrowAngle = glm::radians(MODIFYING_VECTORS_ARROW_ANGLE);
    isDeletable = false;
    beingRendered = false;
}

void ModifyingVectors_PO::buildVertices()
{
    vertices->pushToExistingArray(glm::vec3(0.0f));
    vertices->pushToExistingArray(colorX);
    vertices->pushToExistingArray(glm::vec3(size, .0f, .0f));
    vertices->pushToExistingArray(colorX);
    vertices->allocateNewArray();
    vertices->pushToExistingArray(glm::vec3(0.0f));
    vertices->pushToExistingArray(colorY);
    vertices->pushToExistingArray(glm::vec3(.0f, size, .0f));
    vertices->pushToExistingArray(colorY);
    vertices->allocateNewArray();
    vertices->pushToExistingArray(glm::vec3(0.0f));
    vertices->pushToExistingArray(colorZ);
    vertices->pushToExistingArray(glm::vec3(.0f, .0f, size));
    vertices->pushToExistingArray(colorZ);

    constructTranslationArrows();
    constructScalingArrows();




    for (int j = 0; j < 2; j++) {
        vertices->allocateNewArray();
        glm::vec3 col = (j == 0) ? colorZ : colorX;

        float startAngle = (j == 0) ? MODIFYING_VECTORS_ROTATION_CIRCLE_ANGLE_SEPARATION :
            180.0f + MODIFYING_VECTORS_ROTATION_CIRCLE_ANGLE_SEPARATION;
        float endAngle = (j == 0) ? 180.0f - MODIFYING_VECTORS_ROTATION_CIRCLE_ANGLE_SEPARATION :
            360.0f - MODIFYING_VECTORS_ROTATION_CIRCLE_ANGLE_SEPARATION;

        float angle = (endAngle - startAngle) / (MODIFYING_VECTORS_ROTATION_CIRCLE_SEGMENT_NUM - 1);

        for (int i = 0; i < MODIFYING_VECTORS_ROTATION_CIRCLE_SEGMENT_NUM; i++) {
            float currentAngleRad = glm::radians(startAngle + i * angle);
            vertices->pushToExistingArray(glm::vec3(size / 2.0f * cos(currentAngleRad),
                0.0f,
                size / 2.0f * sin(currentAngleRad)));
            vertices->pushToExistingArray(col);
        }
    }


    constructRotationArrows();


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
        VAOs->addVBO(VBO);
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
        point = glm::vec3(x * size, y * size, z * size);
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
        point = glm::vec3(x * size, y * size, z * size);
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

void ModifyingVectors_PO::constructRotationArrows()
{
    glm::vec3 firstPoint = __zero_vec3;
    glm::vec3 secondPoint = __zero_vec3;
    float radius = MODIFYING_VECTORS_ARROW_LENGTH
        * sin(arrowAngle);
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 col = glm::vec3(0.0f);
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < 2; i++)
    {
        switch (i) {
        case 0:
            firstPoint = vertices->at(15)->at(0);
            secondPoint = vertices->at(15)->at(6);
            col = __color_blue;
            break;
        case 1:
            firstPoint = vertices->at(16)->at(0);
            secondPoint = vertices->at(16)->at(6);
            col = __color_red;
            break;
        }
        glm::vec3 vecNormDir = glm::normalize(secondPoint - firstPoint);
        point = firstPoint;
        vertices->allocateNewArray();
        vertices->pushToExistingArray(point);
        vertices->pushToExistingArray(col);
        glm::vec3 axis = glm::normalize(secondPoint - firstPoint);
        glm::vec3 orthVec = __y_norm_vec3;
        glm::vec3 perpVec = glm::normalize(glm::cross(axis, orthVec));

        for (float j = 0.0f; j < 450.0f; j += MODIFYING_VECTORS_ARROW_CIRCLE_INTERVAL) {
            float angle = glm::radians(j);
            glm::vec3 offset = MODIFYING_VECTORS_ARROW_LENGTH * axis + radius * (cos(angle) * perpVec +
                sin(angle) * orthVec);
            vertices->pushToExistingArray(point + offset);
            vertices->pushToExistingArray(col);

        }
    }
}


void ModifyingVectors_PO::build()
{
    vertices->clear();
    VAOs->clear();
    buildVertices();
    buildVAOs();
    generateModelMatrix();
}

void ModifyingVectors_PO::rebuild()
{
    generateModelMatrix();
}

void ModifyingVectors_PO::generateModelMatrix()
{

    translationVector = origin;
    /*scalingVector = glm::vec3(length / initial_length,
        height / initial_height, width / initial_width);*/

    glm::vec3 centerOffset = glm::vec3(length / 2.0f, height / 2.0f, width / 2.0f);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translationVector + centerOffset);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::translate(modelMatrix, -centerOffset);
   


    //modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::translate(modelMatrix, translationVector);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    
    //modelMatrix = glm::scale(modelMatrix, scalingVector);


    inverseModelMatrix = glm::inverse(modelMatrix);
    
}

void ModifyingVectors_PO::draw()
{
    if (!beingRendered)
    {
        return;
    }
    if (rebuiltExpected) {
        rebuild();
        rebuiltExpected = false;
    }
    glDisable(GL_DEPTH_TEST);
    shader->bind();
    updateModelMatrix();


    switch (type) {
        case Translation:
        for (int i = 0; i < 3; i++) {
            glBindVertexArray(VAOs->at(i));
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
            glBindVertexArray(0);
        }
        for (int i = 3; i < 6; i++) {
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
            for (int i = 6; i < 15; i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
                glBindVertexArray(0);
            }
            break;
        case Rotation:
            for (int i = 15; i < 17; i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
                glBindVertexArray(0);
            }
            for (int i = 17; i < 19; i++) {
                glBindVertexArray(VAOs->at(i));
                glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices->at(i)->size() / 2));
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
    glm::vec4 vLocalFrame = inverseModelMatrix * glm::vec4(v, 1.0f);
    if (type == Translation || type == Scaling)
    {
        if ((0.0f < vLocalFrame.x && size > vLocalFrame.x) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.y &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.y) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.z &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.z))
        {
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }
        if ((0.0f < vLocalFrame.y && size > vLocalFrame.y) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.x &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.x) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.z &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.z))
        {
            return glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if ((0.0f < vLocalFrame.z && size > vLocalFrame.z) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.y &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.y) &&
            (MODIFYING_VECTORS_BOUNDARY_HALF_SIZE > vLocalFrame.x &&
                -MODIFYING_VECTORS_BOUNDARY_HALF_SIZE < vLocalFrame.x))
        {
            // neg z dir
            return glm::vec3(0.0f, 0.0f, 1.0f);
        }
        return glm::vec3(0.0f);
    }
    else 
    {
        float startAngle = 0.0f;
        float endAngle = 360.f;
        int numSegmentsAdjusted = 2 *
            (MODIFYING_VECTORS_ROTATION_CIRCLE_SEGMENT_NUM - 1);
        float angle = (endAngle - startAngle) / numSegmentsAdjusted;
        for (int i = 0; i < numSegmentsAdjusted; i++) {
            float currentAngleRad = glm::radians(startAngle + i * angle);

            glm::vec3 point = glm::vec3(size / 2.0f * cos(currentAngleRad),
                0.0f,
                size / 2.0f * sin(currentAngleRad));
            if (glm::length(point - glm::vec3(vLocalFrame)) < MODIFYING_VECTORS_ROTATION_BOUNDARY_DISTANCE_CHECK)
            {
                return glm::vec3(1.0f, 0.0f, 0.0f);
            }
        }
        return glm::vec3(0.0f);
    }
}
