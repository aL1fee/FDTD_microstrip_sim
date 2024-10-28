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
    cameraTarget = target;  
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
    float rotationRadius = glm::length(cameraPos - point);
    _viewMatrixChanged = true;
    offsetX *= mouseSensitivity * 3.0f;
    offsetY *= mouseSensitivity * 3.0f;

    std::cout << "pitch: " << pitch << std::endl;
    std::cout << "yaw: " << yaw << std::endl;

    std::cout << rotationRadius << std::endl;

    //pitch += offsetY;
    yaw += offsetX;

    float pitchOrigin = 45.0f + pitch;
    float yawOrigin = 180.0f + yaw;

    std::cout << "camPos before: " << glm::to_string(cameraPos) << std::endl;

    cameraPos.x = point.x + rotationRadius * cos(glm::radians(pitchOrigin)) * cos(glm::radians(yawOrigin));
    cameraPos.z = point.z + rotationRadius * cos(glm::radians(pitchOrigin)) * sin(glm::radians(yawOrigin));
    cameraPos.y = point.y + rotationRadius * sin(glm::radians(pitchOrigin));

    std::cout << "camPos after: " << glm::to_string(cameraPos) << std::endl;

    updateVectors();



   /* float pitchCamPos = pitch - 90.0f;
    float yawCamPos = yaw - 180.0f;

    std::cout << "pitch: " << pitch << std::endl;
    std::cout << "yaw: " << yaw << std::endl;*/

    //updateVectors();

    //if (offsetX > 0) {
    //    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt + cameraRight * abs(offsetX) * dt;
    //}
    //else if (offsetX < 0) {
    //    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt + cameraRight * abs(offsetX) * dt;
    //}

    //std::cout << "cam X: " << cameraPos.x << std::endl;
    //std::cout << "cam Z: " << cameraPos.z << std::endl;
    //std::cout << "cam Y: " << cameraPos.y << std::endl;

    //cameraPos.x = rotationRadius * cos(glm::radians(pitchCamPos)) * cos(glm::radians(yawCamPos));
    //cameraPos.z = rotationRadius * cos(glm::radians(pitchCamPos)) * sin(glm::radians(yawCamPos));
    //cameraPos.y = rotationRadius * sin(glm::radians(pitchCamPos));

    ////std::cout << "rotationRadius: " << rotationRadius << std::endl;
    //std::cout << "cam X: " << cameraPos.x << std::endl;
    //std::cout << "cam Z: " << cameraPos.z << std::endl;
    //std::cout << "cam Y: " << cameraPos.y << std::endl;


    //void Camera::updateVectors()
    //{
    //    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //    cameraFront.y = sin(glm::radians(pitch));
    //    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //    cameraFront = glm::normalize(cameraFront);
    //    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    //    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    //}



    //std::cout << "pitch: " << pitch << std::endl;
    //std::cout << "yaw: " << yaw << std::endl;

    ////pitch += offsetY * dt;
    ////yaw += offsetX * dt;
    //float pitchDelta = offsetY;
    //float yawDelta = offsetX;

    //std::cout << "camLoc1: " << glm::to_string(cameraPos) << std::endl;
    ////std::cout << "rotation radius: " << rotationRadius << std::endl;
    //std::cout << "HERE: " << 
    //    rotationRadius * cos(glm::radians(pitchDelta)) * cos(glm::radians(yawDelta)) << std::endl;

    ////cameraPos.x += cos(glm::radians(pitchDelta)) * cos(glm::radians(yawDelta));
    ////cameraPos.z += cos(glm::radians(pitchDelta)) * sin(glm::radians(yawDelta));
    ////cameraPos.y += sin(glm::radians(pitchDelta));

    //std::cout << "camLoc2: " << glm::to_string(cameraPos) << std::endl;

    //updateVectors();



    //// Calculate the angles based on the offsets
    //float theta = glm::radians(offsetY) * dt; // Vertical angle
    //float phi = glm::radians(offsetX) * dt; // Horizontal angle

    //// Current camera position relative to the point
    //glm::vec3 direction = cameraPos - point;

    //// Convert to spherical coordinates
    //float currentTheta = acos(direction.z / rotationRadius);
    //float currentPhi = atan2(direction.y, direction.x);

    //// Update the angles
    //currentTheta += theta;
    //currentPhi += phi;

    //// Convert back to Cartesian coordinates
    //cameraPos.x = point.x + rotationRadius * sin(currentTheta) * cos(currentPhi);
    //cameraPos.y = point.y + rotationRadius * sin(currentTheta) * sin(currentPhi);
    //cameraPos.z = point.z + rotationRadius * cos(currentTheta);

    //// Update the camera's view matrix or other relevant properties here

    //updateVectors();


    //_viewMatrixChanged = true;
    //offsetX *= mouseSensitivity;
    //offsetY *= mouseSensitivity;

    //yaw = glm::degrees(atan2(cameraFront.z, cameraFront.x));

    //_viewMatrixChanged = true;
    //offsetX *= mouseSensitivity;
    //offsetY *= mouseSensitivity;
    //yaw += offsetX;
    //pitch += offsetY;
    //if (pitch > 89.0f) {
    //    pitch = 89.0f;
    //}
    //if (pitch < -89.0f) {
    //    pitch = -89.0f;
    //}
    //updateVectors();


}
