#include "Camera.h"

Camera::Camera(glm::vec3& eye, glm::vec3& target, glm::vec3& up) : eye(eye), target(target), cameraUp(up) {
	direction = glm::normalize(eye - target); //ociste - glediste jer pise da je zadano negativnom z osi
	cameraX = glm::normalize(glm::cross(up, direction)); // u knjizi je opisan lijevi koord sistem
	cameraY = glm::normalize(glm::cross(direction, cameraX)); //camera y pozicija
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 sol = glm::lookAt(eye, target, cameraUp); //not sure
	return sol;
}

glm::mat4 Camera::getPerspectiveMatrix(int width, int height) {
	glm::mat4 sol = glm::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
	return sol;
}


float Camera::getCameraSpeed() {
	return cameraSpeed;
}
