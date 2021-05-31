#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Curve.h"

class Bezier {
private:
	glm::mat4 coeffs = glm::mat4(
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0);
	float t;

	std::vector<int> computeFactors(int);


public:

	Bezier();
	~Bezier();


	const glm::vec4& parVector(float);

	const glm::mat4& getCoeffs();

	std::vector<glm::vec3> interPoints(std::vector<glm::vec3>&); //points must have length 4

	void draw_bezier(Curve&, int divs);

	glm::vec3 getPoint(float t, std::vector<glm::vec3>& controls);

};