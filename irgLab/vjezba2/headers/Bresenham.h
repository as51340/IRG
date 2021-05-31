#pragma once

#include "Grafika.h"

/*
* Class with several functions that are used in Bresenham line drawing
*/
class Bresenham
{

private:
	int width, height;
	glm::vec3 col_red = glm::vec3(255, 0, 0);
	glm::vec3 col_green = glm::vec3(0, 255, 0);

public:

	Bresenham(int width, int height) : width(width), height(height) {};

	/*
	* We are allowing user not to specify width and height but in that case he/she shouldn't use functions draw_central_green_segment and
	* draw_suther_line
	*/
	Bresenham() {};

	~Bresenham();

	void swap(int* elem1, int* elem2);

	void bresenham_loop(Grafika& graphics, int xs, int xe, int correction, int yc, int yf, int a, short inc, bool cond, bool swap,
		glm::vec3& col);

	void bresenham_draw_int2(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col);

	void bresenham_draw_int3(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col);

	void draw_line(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col);

	void draw_suther_line(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col);

	glm::vec2 intersect_with_y(double coeff, int xs, int ys, int y);

	glm::vec2 intersect_with_x(double coeff, int xs, int ys, int x);

	void draw_central_green_segment(Grafika& graphics);

};