#pragma once

#include "Tranform.h"

class Light : public Transform {
public:
	float ir, ig, ib; //intensities

	glm::vec3 ii, ia;

	glm::vec3 position;

	void print();
};