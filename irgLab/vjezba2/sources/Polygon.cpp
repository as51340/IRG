#include "Polygon.h"

void PolygonIRG::drawConvexPolygon(Grafika& graphics, std::vector<PolyElem>& polyElements, Bresenham& bresenham) {
	int n = polyElements.size();
	int i, i0 = n - 1; //first draw last line(from vertex n-1 to vertex 0(actually is 1 but because of the indexing)

	for (i = 0; i < n; i++) {
		bresenham.draw_line(graphics, polyElements[i0].ver.x, polyElements[i0].ver.y, polyElements[i].ver.x, polyElements[i].ver.y,
			glm::vec3(255, 0, 0)); //draw red line
		i0 = i; //not ++ because it is initially on n-1 position
	}
}

void PolygonIRG::calculateCoeffsForConvexPolygon(std::vector<PolyElem>& polyElements) {
	int i0, i, n;
	n = polyElements.size();
	i0 = n - 1;
	for (i = 0; i < n; i++) {
		glm::vec3 first_vec = glm::vec3(polyElements[i0].ver.x, polyElements[i0].ver.y, 1);
		glm::vec3 second_vec = glm::vec3(polyElements[i].ver.x, polyElements[i].ver.y, 1);
		polyElements[i0].edge = glm::cross(first_vec, second_vec);
		polyElements[i0].left = polyElements[i0].ver.y < polyElements[i].ver.y; //true if it is left, false if it is right edge
		i0 = i;
	}
}

/*
* Scan line algorithm
*/
void PolygonIRG::fillConvexPolygon(std::vector<PolyElem>& polyElements, Bresenham& bresenham, Grafika& graphics, bool orientation) {
	int i, i0, y, n = polyElements.size(), x_min, x_max, y_min, y_max;
	double l, d, x;
	if (!orientation) {
		reverse(polyElements.begin(), polyElements.end());
	}
	x_min = x_max = polyElements[0].ver.x;
	y_min = y_max = polyElements[0].ver.y;
	for (i = 1; i < n; i++) {
		if (polyElements[i].ver.x < x_min) x_min = polyElements[i].ver.x;
		if (polyElements[i].ver.x > x_max) x_max = polyElements[i].ver.x;
		if (polyElements[i].ver.y < y_min) y_min = polyElements[i].ver.y;
		if (polyElements[i].ver.y > y_max) y_max = polyElements[i].ver.y;
	}
	for (y = y_min; y <= y_max; y++) { // cast ray
		l = x_min;
		d = x_max;
		i0 = n - 1;
		// find rightmost intersection point with left edge
		// find leftmost intersection point with right edge
		for (i = 0; i < n; i0 = i++) {
			//if edge is horizontal
			if (glm::abs(polyElements[i0].edge.x) < 1e-5) {
				if (glm::abs(polyElements[i0].ver.y - y) < 1e-5) {
					if (polyElements[i0].ver.x < polyElements[i].ver.x) {
						l = polyElements[i0].ver.x;
						d = polyElements[i].ver.x;
					}
					else {
						l = polyElements[i].ver.x;
						d = polyElements[i0].ver.x;
					}
					break;
				}
			}
			else { //regular edge find intersection point
				x = ((double)-polyElements[i0].edge.y * (double)y - polyElements[i0].edge.z) / (double)polyElements[i0].edge.x;
				if (polyElements[i0].left) {
					if (l < x) l = x;
				}
				else {
					if (d > x) d = x;
				}
			}
		}
		/*if (!orientation) {
			double tmp = l;
			l = d;
			d = tmp;
		} */
		bresenham.draw_line(graphics, glm::round(l), y, glm::round(d), y, glm::vec3(255, 255, 255)); //draw line from left intersection
		//to the right intersection
	}
}

void PolygonIRG::checkConvexPolygon(std::vector<PolyElem>& polyElements, bool& convex, bool& orientation) {
	int i, i0, r, n = polyElements.size();
	int below = 0, above = 0, on = 0;
	i0 = n - 2;
	for (i = 0; i < n; i++, i0++) {
		if (i0 >= n) i0 = 0;
		glm::vec3 homo_coordinates_vertex(polyElements[i].ver.x, polyElements[i].ver.y, 1);
		r = glm::dot(polyElements[i0].edge, homo_coordinates_vertex); //calculate dot product, we already have coefficients for edges
		if (glm::abs(r) < 1e-7) on++;
		else if (r < 0) below++;
		else above++;
		convex = false; //concave
		orientation = false; //counterclockwise
		if (below == 0) {
			convex = true;
		}
		else if (above == 0) {
			orientation = true; //clockwise
			convex = true;
		}
	}
}

bool PolygonIRG::pointPolygonRelation(glm::vec2& vertex, std::vector<PolygonIRG::PolyElem>& polygon, bool& orientation) {
	int n = polygon.size();
	glm::vec3 homo_coord(vertex.x, vertex.y, 1.0);
	for (int i = 0; i < n; i++) {
		double dot = glm::dot(polygon[i].edge, homo_coord);
		if ((orientation == true && dot > 1e-5) || (orientation == false && dot < 1e-5)) {
			return false;
		}
	}
	return true;
}

PolygonIRG::PolyElem PolygonIRG::createNewPolygonElement(glm::vec2& last_vertex, glm::vec2& vertex, bool orientation) {
	glm::vec3 new_edge = glm::cross(glm::vec3(last_vertex.x, last_vertex.y, 1.0), glm::vec3(vertex.x, vertex.y, 1.0));
	PolygonIRG::PolyElem new_elem;
	new_elem.ver = last_vertex;
	new_elem.edge = new_edge;
	new_elem.left = last_vertex.y < vertex.y; // 1 for left, 0 for right edge, so we don't have to use function calculateCoeffs
	/*if (!orientation) {
		new_elem.left = !new_elem.left;
	}*/
	new_elem.end_ver = vertex;
	return new_elem;
}
