#include "glmPractise.h"
#include <iostream>

glm::vec2 findReflectedVector(glm::vec2 n, glm::vec2 m)
{
	double cos_a = glm::dot(glm::normalize(n), glm::normalize(m));
	glm::vec2 k = glm::normalize(n) * glm::length(m) * cos_a;
	glm::vec2 p = k - m;
	return m + 2 * p;
}

double calculateLambdaParametricLineEquation(glm::vec2 start_point, glm::vec2 end_point, glm::vec2 target_point)
{
	glm::mat2 l_matrix = glm::mat2(end_point - start_point, start_point);
	return (target_point * glm::inverse(l_matrix)).x;
}

/*
* return 0 if point is beyond line, 1 if point is on the line and 2 if line is below line
*/
short getRelationBetweenPointAndLine(glm::vec2 point, glm::vec3 line_coeff, double homo_param)
{
	glm::vec3 homo_coordinates = glm::vec3(point.x, point.y, homo_param);
	double sol = glm::dot(homo_coordinates, line_coeff);
	if (glm::abs(sol) < 1e-7) return 1;
	if (homo_param > 0) {
		if (sol < 0) return 0;
		return 2;
	}
	else if (sol > 0) {
		return 0;
	}
	return 2;
}

/*
* return 0 if point is beyond plane, 1 if point is in the plane and 2 if point is below plane
*/
short getRelationBetweenPointAndPlane(glm::vec3 point, glm::vec4 line_coeff, double homo_param)
{
	glm::vec4 homo_coordinates = glm::vec4(point.x, point.y, point.z, homo_param);
	double sol = glm::dot(homo_coordinates, line_coeff);
	if (glm::abs(sol) < 1e-7) return 1;
	if (homo_param > 0) {
		if (sol < 0) return 0;
		return 2;
	}
	else if (sol > 0) {
		return 0;
	}
	return 2;
}

glm::vec3 calculateBaricentricCoordinates(glm::mat3 triangle_points, glm::vec3 target_point)
{
	return glm::inverse(triangle_points) * target_point;
}

/*
* return 0 if point is in triangle, 1 if point is one of the vertices, 2 if it is on some of the edges, 3 if it is outside triangle
*/
short getRelationBetweenPointAndTriangle(glm::mat3 triangle_points, glm::vec3 target_point)
{
	glm::vec3 baric_coord = calculateBaricentricCoordinates(triangle_points, target_point);
	if (baric_coord.x > 0 && baric_coord.x < 1 && baric_coord.y > 0 && baric_coord.y < 1 && baric_coord.z > 0 && baric_coord.z < 1)
		return 0;
	if (baric_coord.x < 0 || baric_coord.x > 1 || baric_coord.y < 0 || baric_coord.y > 1 || baric_coord.z < 0 || baric_coord.z > 1)
		return 3;
	if (baric_coord.x == 1 || baric_coord.y == 1 || baric_coord.z == 1) return 1;

	if ((baric_coord.x == 0 && baric_coord.y != 0 && baric_coord.z != 0) || (baric_coord.y == 0 && baric_coord.x != 0 && baric_coord.z != 0) ||
		(baric_coord.z == 0 && baric_coord.y != 0 && baric_coord.x != 0)) return 2;
}

glm::vec3 intersectLineAndPlane(glm::vec3 startLinePoint, double line_lambda, glm::vec3 dir_line, glm::vec4 plane_coeff)
{
	glm::vec3 norm_plane = glm::vec3(plane_coeff.x, plane_coeff.y, plane_coeff.z);
	double temp_dot = glm::dot(norm_plane, dir_line);
	if (temp_dot == 0) {
		throw "Line and plane do not intersect!";
	}
	else {
		double temp_dot2 = glm::dot(norm_plane, startLinePoint);
		double lambda = (-plane_coeff.w - temp_dot2) / temp_dot;
		return lambda * dir_line + startLinePoint;
	}
}



