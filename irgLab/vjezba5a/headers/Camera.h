#pragma once

#include "Tranform.h"
#include <glm/gtc/matrix_transform.hpp>



class Camera : Transform {
private:
	float cameraSpeed = 0.05f; // adjust accordingly
public:

	glm::vec3 eye, target, direction, cameraX, cameraY, cameraUp;


	glm::mat4 getPerspectiveMatrix(int, int);

	virtual glm::mat4 getViewMatrix();

	Camera(glm::vec3&, glm::vec3&, glm::vec3&);

	float getCameraSpeed();

};