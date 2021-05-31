#include "Bresenham.h"

Bresenham::~Bresenham()
{
}

void Bresenham::swap(int* elem1, int* elem2) {
	int tmp = *elem1;
	*elem1 = *elem2;
	*elem2 = tmp;
}

/*
* Inc is for adding or subtracting 1 from yc -> is ye is larger or fewer
*/
void Bresenham::bresenham_loop(Grafika& graphics, int xs, int xe, int correction, int yc, int yf, int a, short inc, bool cond, bool swap,
	glm::vec3& col) {
	for (int x = xs; x <= xe; x++) {
		if (swap)  graphics.osvijetliFragment(yc, x, col);
		else graphics.osvijetliFragment(x, yc, col);
		yf = yf + a;
		if ((cond && yf >= 0) || (!cond && yf <= 0)) {
			yf = yf + correction;
			yc = yc + inc;
		}
	}
}

/*
* for angles between 0 and 90, if angle is larger than 45 deg then y and x are swapped
*/
void Bresenham::bresenham_draw_int2(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col) {
	int yc, correction, a, yf;
	bool swap_var = false;
	if (ye - ys > xe - xs) { //if angle is between 45 and 90 degrees
		swap(&xe, &ye);
		swap(&xs, &ys);
		swap_var = true;
	}
	a = 2 * (ye - ys);
	yc = ys;
	yf = -(xe - xs);
	correction = -2 * (xe - xs);
	bresenham_loop(graphics, xs, xe, correction, yc, yf, a, 1, true, swap_var, col);
}

/*
* For angles between 0 and -90 deg
*/
void Bresenham::bresenham_draw_int3(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col) {
	int yc, correction, a, yf;
	bool swap_var = false;
	if (-(ye - ys) > xe - xs) {
		swap(&xe, &ys);
		swap(&xs, &ye);
		swap_var = true;
	}
	a = 2 * (ye - ys);
	yc = ys;
	yf = xe - xs;
	correction = 2 * (xe - xs);
	bresenham_loop(graphics, xs, xe, correction, yc, yf, a, -1, false, swap_var, col);
}



void Bresenham::draw_line(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col) {
	if (xs <= xe) {
		if (ys <= ye) {
			bresenham_draw_int2(graphics, xs, ys, xe, ye, col);
		}
		else {
			bresenham_draw_int3(graphics, xs, ys, xe, ye, col);
		}
	}
	else {
		if (ys >= ye) {
			bresenham_draw_int2(graphics, xe, ye, xs, ys, col);
		}
		else {
			bresenham_draw_int3(graphics, xe, ye, xs, ys, col);
		}
	}
}

void Bresenham::draw_suther_line(Grafika& graphics, int xs, int ys, int xe, int ye, glm::vec3& col) {
	double y_max = 0.75 * height;
	double y_min = 0.25 * height;
	double x_min = 0.25 * width;
	double x_max = 0.75 * width;
	std::vector<bool> code_start = std::vector<bool>(4, 0);
	std::vector<bool> code_end = std::vector<bool>(4, 0);

	bool all_zeros_start = true;
	if (ys > y_max) {
		code_start[0] = 1;
		all_zeros_start = false;
	}
	if (ys < y_min) {
		code_start[1] = 1;
		all_zeros_start = false;
	}
	if (xs > x_max) {
		code_start[2] = 1;
		all_zeros_start = false;
	}
	if (xs < x_min) {
		code_start[3] = 1;
		all_zeros_start = false;
	}

	bool all_zeros_end = true;
	if (ye > y_max) {
		code_end[0] = 1;
		all_zeros_end = false;
	}
	if (ye < y_min) {
		code_end[1] = 1;
		all_zeros_end = false;
	}
	if (xe > x_max) {
		code_end[2] = 1;
		all_zeros_end = false;
	}
	if (xe < x_min) {
		code_end[3] = 1;
		all_zeros_end = false;
	}


	if (all_zeros_start == true && all_zeros_end == true) {
		draw_line(graphics, xs, ys, xe, ye, col_red);
		return;
	}
	bool and_operator = false;
	for (int i = 0; i < 4; i++) { //if and operation for two codes is different than 0000
		if (code_start[i] == 1 && code_end[i] == 1) return;
	}

	glm::vec2 start_point = glm::vec2(xs, ys);
	glm::vec2 end_point = glm::vec2(xe, ye);

	double coeff;
	if (xe - xs == 0) { // vertical line and we will have no intersection points, TODO
		coeff = 0.0;
	}
	else {
		coeff = ((double)ye - ys) / ((double)xe - xs);
	}


	for (int i = 0; i < 4; i++) {
		if (code_start[i] == 1) {
			if (i == 0) {
				start_point = intersect_with_y(coeff, xe, ye, y_max);
			}
			else if (i == 1) {
				start_point = intersect_with_y(coeff, xe, ye, y_min);
			}
			else if (i == 2) {
				start_point = intersect_with_x(coeff, xe, ye, x_max);
			}
			else if (i == 3) {
				start_point = intersect_with_x(coeff, xe, ye, x_min);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (code_end[i] == 1) {
			if (i == 0) {
				end_point = intersect_with_y(coeff, xs, ys, y_max);
			}
			else if (i == 1) {
				end_point = intersect_with_y(coeff, xs, ys, y_min);
			}
			else if (i == 2) {
				end_point = intersect_with_x(coeff, xs, ys, x_max);
			}
			else if (i == 3) {
				end_point = intersect_with_x(coeff, xs, ys, x_min);
			}
		}
	}
	//because of small deviations
	if (start_point.x < x_min) {
		start_point.x = x_min;
	}
	else if (start_point.x > x_max) {
		start_point.x = x_max;
	}
	if (end_point.x < x_min) {
		end_point.x = x_min;
	}
	else if (end_point.x > x_max) {
		end_point.x = x_max;
	}
	if (start_point.y < y_min) {
		start_point.y = y_min;
	}
	else if (start_point.y > y_max) {
		start_point.y = y_max;
	}
	if (end_point.y < y_min) {
		end_point.y = y_min;
	}
	else if (end_point.y > y_max) {
		end_point.y = y_max;
	}
	draw_line(graphics, start_point.x, start_point.y, end_point.x, end_point.y, col_red);
}

glm::vec2 Bresenham::intersect_with_y(double coeff, int xs, int ys, double y) {
	if (glm::abs(coeff) < 1e-7) { //if coeff is zero
		return glm::vec2(xs, (double)y);
	}
	else {
		return glm::vec2((y + coeff * xs - ys) / coeff, (double)y);
	}
}

glm::vec2 Bresenham::intersect_with_x(double coeff, int xs, int ys, double x) {
	return glm::vec2((double)x, coeff * ((double)x - xs) + ys);
}

void Bresenham::draw_central_green_segment(Grafika& graphics) {
	glm::vec2 a = glm::vec2(0.25 * width, 0.75 * height);
	glm::vec2 b = glm::vec2(0.75 * width, 0.75 * height);
	glm::vec2 c = glm::vec2(0.25 * width, 0.25 * height);
	glm::vec2 d = glm::vec2(0.75 * width, 0.25 * height);

	draw_line(graphics, a.x, a.y, b.x, b.y, col_green);
	draw_line(graphics, b.x, b.y, d.x, d.y, col_green);
	draw_line(graphics, d.x, d.y, c.x, c.y, col_green);
	draw_line(graphics, c.x, c.y, a.x, a.y, col_green);
}