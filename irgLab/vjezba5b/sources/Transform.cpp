#include "Tranform.h"

Transform::Transform() {};
Transform::~Transform() {};

glm::mat4 Transform::createTransformMatrix(float x, float y, float z) {
	glm::mat4 tMat = glm::mat4(1.0);
	tMat[3][0] = x;
	tMat[3][1] = y;
	tMat[3][2] = z;
	//std::cout << glm::to_string(tMat);
	return tMat;
}

glm::mat4 Transform::createScalingMatrix(float M) {
	glm::mat4 tMat = glm::mat4(1);
	if (glm::abs(M) < 1e-7) return tMat;
	tMat[0][0] = 2.0 / M;
	tMat[1][1] = 2.0 / M;
	tMat[2][2] = 2.0 / M;
	//std::cout << glm::to_string(tMat);
	return tMat;
}

glm::mat4 Transform::getModelMatrix() {
	return model;
}

glm::mat4 Transform::getViewMatrix() {
	glm::mat4 sol = glm::mat4(1); //create jedinicna
	return sol;
}

void Transform::updateModel(glm::mat4& matrix) {
	model = matrix * model;
}

//convention multiplying jedinicna
glm::mat4 Transform::translate3D(glm::vec3& translateVector) {
	glm::mat4 jed = glm::mat4(1);
	jed[3][0] = translateVector.x;
	jed[3][1] = translateVector.y;
	jed[3][2] = translateVector.z;
	return jed;
}

glm::mat4 Transform::scale3D(glm::vec3& scaleVector) {
	glm::mat4 jed = glm::mat4(1);
	jed[0][0] = scaleVector.x;
	jed[1][1] = scaleVector.y;
	jed[2][2] = scaleVector.z;
	return jed;
}

glm::mat4 Transform::lookAtMatrix(glm::vec3& eye, glm::vec3& center, glm::vec3& viewUp) {
	glm::mat4 jed = glm::mat4(1);
	glm::vec3 cameraDirection = glm::normalize(eye - center); //ociste - glediste jer pise da je zadano negativnom z osi
	glm::vec3 cameraRight = glm::normalize(glm::cross(viewUp, cameraDirection)); // u knjizi je opisan lijevi koord sistem
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight)); //camera y pozicija
	jed[0][0] = cameraRight.x;
	jed[1][0] = cameraRight.y;
	jed[2][0] = cameraRight.z;

	jed[0][1] = cameraUp.x;
	jed[1][1] = cameraUp.y;
	jed[2][1] = cameraUp.z;

	jed[0][2] = cameraDirection.x;
	jed[1][2] = cameraDirection.y;
	jed[2][2] = cameraDirection.z;

	glm::mat4 nov = glm::mat4(1);
	nov[3][0] = -eye.x;
	nov[3][1] = -eye.y;
	nov[3][2] = -eye.z;
	return jed * nov;
}

glm::mat4 Transform::frustum(double l, double r, double b, double t, double n, double f) {
	glm::mat4 sol = glm::mat4(1.0);
	sol[0][0] = (2.0 * n) / (r - l);
	sol[1][1] = (2.0 * n) / (t - b);
	sol[2][2] = -(f + n) / (f - n);
	sol[2][3] = -1.0;
	sol[3][2] = (-2.0 * f * n) / (f - n);
	sol[2][0] = (r + l) / (r - l);
	sol[2][1] = (t + b) / (t - b);
	sol[3][3] = 0.0;
	return sol;
}

