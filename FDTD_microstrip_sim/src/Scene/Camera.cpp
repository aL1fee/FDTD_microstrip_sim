#include "Camera.h"

void Camera::updateVectors()
{
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp)); 
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::computeInitialYaw()
{
    yaw = glm::degrees(atan2(cameraFront.z, cameraFront.x));
}

void Camera::computeInitialPitch()
{
    pitch = glm::degrees(asin(cameraFront.y));
}

Camera::Camera(glm::vec3 pos, glm::vec3 target)
{
    cameraPos = pos;
    cameraTarget = target; // not using cameraTarget!
    cameraRight = glm::vec3(0.0f);
    cameraFront = glm::normalize(glm::vec3(CAMERA_INITIAL_FRONT_X, CAMERA_INITIAL_FRONT_Y, CAMERA_INITIAL_FRONT_Z));
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraUp = worldUp;
    cameraSpeed = CAMERA_SPEED;
    mouseSensitivity = MOUSE_SENSITIVITY;
    yaw = INITIAL_YAW;
    pitch = INITIAL_PITCH;
    zoom = INITIAL_ZOOM;
    computeInitialYaw();
    computeInitialPitch();
    updateVectors();
    yawCamToOrigin = 0.0f; 
    pitchCamToOrigin = 0.0f;
}

glm::mat4 Camera::lookAt() const
{
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}

void Camera::updatePos(glm::vec3 pos)
{
    cameraPos = pos;
}

void Camera::updateTarget(glm::vec3 target)
{
    cameraTarget = target;
}

void Camera::updateRight(glm::vec3 right)
{
    cameraRight = right;
}

void Camera::moveForward(float dt)
{
    _viewMatrixChanged = true;
    cameraPos += cameraFront * cameraSpeed * dt;
}

void Camera::moveBackward(float dt)
{
    _viewMatrixChanged = true;
    cameraPos -= cameraFront * cameraSpeed * dt;
}

void Camera::moveRight(float dt)
{
    _viewMatrixChanged = true;
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
}

void Camera::moveLeft(float dt)
{
    _viewMatrixChanged = true;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
}

void Camera::processMouseRotation(float offsetX, float offsetY)
{
    _viewMatrixChanged = true;
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;
    yaw += offsetX;
    pitch += offsetY;
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    updateVectors();
}

void Camera::processMouseTranslation(float offsetX, float offsetY, float dt)
{
    _viewMatrixChanged = true;
    offsetX *= mouseSensitivity * CAMERA_TRANSLATION_SENSITIVITY;
    offsetY *= mouseSensitivity * CAMERA_TRANSLATION_SENSITIVITY;
    //if (abs(offsetX) > abs(offsetY)) {
        if (offsetX > 0) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt + cameraRight * abs(offsetX) * dt;
        }
        else if (offsetX < 0) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt + cameraRight * abs(offsetX) * dt;
        }
    //}
    //else {
        //if (offsetY > 0) {
        //    cameraPos += glm::normalize(glm::cross(cameraFront, cameraRight)) * cameraSpeed * dt;
        //}
        //else if (offsetY < 0) {
        //    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraRight)) * cameraSpeed * dt;
        //}
    //}
}

void Camera::processMouseScroll(float offsetY, float dt)
{
    _viewMatrixChanged = true;
    //zoom -= (float) offsetY;
    //if (zoom < 1.0f) {
    //    zoom = 1.0f;
    //}
    //if (zoom > 45.0f) {
    //    zoom = 45.0f;
    //}
    if (offsetY > 0) {
        cameraPos += cameraFront * cameraSpeed * dt * CAMERA_SCROLL_SENSITIVITY;
    } 
    else if (offsetY < 0) {
        cameraPos -= cameraFront * cameraSpeed * dt * CAMERA_SCROLL_SENSITIVITY;
    }
}

void Camera::processMouseRotationAboutPoint(float offsetX, float offsetY, float dt, glm::vec3 point)
{
    _viewMatrixChanged = true;
    glm::vec3 relativePos = cameraPos - point;
    float rotationRadius = glm::length(relativePos);

    pitchCamToOrigin = glm::degrees(asin(relativePos.y / glm::length(relativePos)));
    yawCamToOrigin = glm::degrees(atan2(relativePos.z, relativePos.x));

    offsetX *= mouseSensitivity * 3.0f;
    offsetY *= mouseSensitivity * 3.0f;

    yawCamToOrigin += offsetX;

    cameraPos.x = point.x + rotationRadius * cos(glm::radians(pitchCamToOrigin)) * cos(glm::radians(yawCamToOrigin));
    cameraPos.z = point.z + rotationRadius * cos(glm::radians(pitchCamToOrigin)) * sin(glm::radians(yawCamToOrigin));
    cameraPos.y = point.y + rotationRadius * sin(glm::radians(pitchCamToOrigin));

    yaw += offsetX;

    updateVectors();
}
