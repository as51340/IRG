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

