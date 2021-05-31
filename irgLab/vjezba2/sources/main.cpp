// Local Headers

// System Headers
#include "Grafika.h"
#include "Polygon.h"
#include "Bresenham.h"
#include "PairHashFunction.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <glm\gtx\string_cast.hpp>

PolygonIRG poly;
Bresenham bresenham;
int width = 97, height = 97;
std::vector<PolygonIRG::PolyElem> polygon;
glm::vec2 last_vertex;
glm::vec3 last_edge;
int numRightMouseClicked = 0;
bool orientation = false; // 0 for counterclockwise, 1 for clockwise
int num_vertices = 0;
bool concavePolygon = false;
glm::vec3 white_col(255, 255, 255);
glm::vec3 red_col(255, 0, 0);
glm::vec3 green_col(0, 255, 0);
glm::vec3 point_color;
std::unordered_set<std::pair<int, int>, PairHashFunction> points;
bool closed_polygon = false;

void temp_function(glm::vec2& vertex) {
	PolygonIRG::PolyElem elem = poly.createNewPolygonElement(last_vertex, vertex, orientation);
	polygon.push_back(elem);
	last_edge = elem.edge;
	last_vertex = vertex;
	num_vertices++;
	std::pair<int, int> pair = std::make_pair(vertex.x, vertex.y);
	if (points.count(pair)) {
		if (num_vertices >= 4) { //three vertices
			closed_polygon = true;
		}
	}
	else {
		points.insert(pair);
	}
}

void klikMisa(int x, int y, int vrsta) {
	glm::vec2 vertex(x, height - y - 1);
	std::cout << glm::to_string(vertex) << std::endl;
	if (vrsta == 0 && numRightMouseClicked == 0) { //if left mouse button is clicked and right button wasn't clicked yet
		if (num_vertices == 0) {
			last_vertex = vertex;
			num_vertices++;
			points.insert(std::make_pair(vertex.x, vertex.y));
		}
		else if (num_vertices == 1) { //already one exists
			temp_function(vertex);
		}
		else if (num_vertices == 2) {
			double dot = glm::dot(glm::vec3(vertex.x, vertex.y, 1), last_edge);
			if (dot < 0) { //if point is below edge, TODO what if dot is 0
				orientation = true;
			}
			temp_function(vertex);
		}
		else {
			double dot = glm::dot(glm::vec3(vertex.x, vertex.y, 1), last_edge);
			std::cout << "Orientation " << orientation << std::endl;
			std::cout << "Dot product " << dot << std::endl;
			if ((orientation == true && dot > 1e-7) || (orientation == false && dot < 1e-7)) {
				//or orientation is counterclockwise and point is below last_edge
				std::cout << "Hello, you entered point with which your polygon becomes concave. Do you want to enter your point anyway "
					<< "or you want to undo(YES for ENTER, NO for UNDO)" << std::endl;
				std::string answer;
				std::cin >> answer;
				std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
				if (answer.compare("YES") == 0) {
					concavePolygon = true; // we will disable all operations except drawing edges
					temp_function(vertex);
				}
			}
			else { //we are allright
				temp_function(vertex);
			}
		}
	}
	else if (vrsta == 1 && closed_polygon) {
		numRightMouseClicked++;
		if (numRightMouseClicked >= 2) { //determine where is point observing to polygon
			if (poly.pointPolygonRelation(vertex, polygon, orientation)) {
				point_color = green_col;
			}
			else {
				point_color = red_col;
			}
			last_vertex = vertex;
		}
	}
}

void reset() { //TODO
	polygon.clear();
	numRightMouseClicked = 0;
	orientation = false;
	num_vertices = 0;
	concavePolygon = false;
}

int main(int argc, char* argv[]) {

	std::cout << argv[0] << std::endl;
	Grafika graphics(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	graphics.registrirajFunkcijuZaKlikMisa(klikMisa);
	graphics.registerResetKey(reset);

	while (graphics.trebaZatvoriti()) {

		//osvjezavanje pozicije razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		graphics.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					graphics.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					graphics.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

		if (numRightMouseClicked == 0) {
			if (num_vertices == 1) {
				graphics.osvijetliFragment(last_vertex.x, last_vertex.y, white_col);
			}
			int polygon_size = polygon.size();
			for (int i = 0; i < polygon_size; i++) {
				bresenham.draw_line(graphics, polygon[i].ver.x, polygon[i].ver.y, polygon[i].end_ver.x, polygon[i].end_ver.y, white_col);
			}
		}
		else if (numRightMouseClicked >= 1 && !concavePolygon) {
			poly.fillConvexPolygon(polygon, bresenham, graphics, orientation);
			if (numRightMouseClicked >= 2) {
				graphics.osvijetliFragment(last_vertex.x, last_vertex.y, point_color);
			}
		}

		graphics.iscrtajRaster();

		//kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		//kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}


	return EXIT_SUCCESS;
}
