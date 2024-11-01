#pragma once

#include <iostream>
//#include <vector>

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include "../Macros.h"

extern bool _viewMatrixChanged;

enum Direction {
	HORIZONTAL,
	VERTICAL
};

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraRight;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float zoom;
	float cameraSpeed;
	float mouseSensitivity;


	float yawCamToOrigin;
	float pitchCamToOrigin;

	void updateVectors();

	void computeInitialYaw();
	void computeInitialPitch();

public:

	Camera(glm::vec3 pos, glm::vec3 target);

	glm::mat4 lookAt() const;

	void updatePos(glm::vec3 pos);
	void updateTarget(glm::vec3 target);
	void updateRight(glm::vec3 right);

	void moveForward(float dt);
	void moveBackward(float dt);
	void moveRight(float dt);
	void moveLeft(float dt);

	glm::vec3 getPos() const { return cameraPos; }
	glm::vec3 getFront() const { return cameraFront; }
	glm::vec3 getRight() const { return cameraRight; }
	glm::vec3 getUp() const { return cameraUp; }
	float getZoom() const { return zoom; }

	void processMouseRotation(float offsetX, float offsetY);
	void processMouseTranslation(float offsetX, float offsetY, float dt);
	void processMouseScroll(float offsetY, float dt);
	void processMouseRotationAboutPoint(float offsetX,
		float offsetY, float dt, glm::vec3 point,
		Direction dir);

	void modifyCameraSpeed(float value) { cameraSpeed = value; }

};