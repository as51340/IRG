#pragma once

// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/ext.hpp"


// Standard Headers
#include <cstdio>
#include <cstdlib>

glm::vec2 findReflectedVector(glm::vec2 main, glm::vec2 ref);

double calculateLambdaParametricLineEquation(glm::vec2 start_point, glm::vec2 end_point, glm::vec2 target_point);

/*
* return 0 if point is beyond line, 1 if point is on the line and 2 if line is below line
*/
short getRelationBetweenPointAndLine(glm::vec2 point, glm::vec3 line_coeff, double homo_param);

/*
* return 0 if point is beyond plane, 1 if point is in the plane and 2 if point is below plane
*/
short getRelationBetweenPointAndPlane(glm::vec3 point, glm::vec4 line_coeff, double homo_param);

glm::vec3 calculateBaricentricCoordinates(glm::mat3 triangle_points, glm::vec3 target_point);

/*
* return 0 if point is in triangle, 1 if point is one of the vertices, 2 if it is on some of the edges, 3 if it is outside triangle
*/
short getRelationBetweenPointAndTriangle(glm::mat3 triangle_points, glm::vec3 target_point);

glm::vec3 intersectLineAndPlane(glm::vec3 startLinePoint, double line_lambda, glm::vec3 dir_line, glm::vec4 plane_coeff);

