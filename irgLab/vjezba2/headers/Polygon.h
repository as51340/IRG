#pragma once

#include "Bresenham.h"

#include <vector>
#include <algorithm>


class PolygonIRG {

public:

	typedef struct {
		glm::vec2 ver;
		glm::vec3 edge;
		glm::vec2 end_ver;
		bool left;
	} PolyElem;



	/*
	* Draws only edges for some polygon that is specified with PolyElem
	*/
	void drawConvexPolygon(Grafika& graphics, std::vector<PolyElem>&, Bresenham& bresenham);

	/*
	* Calculates coefficient for each of n edges in polygon
	*/
	void calculateCoeffsForConvexPolygon(std::vector<PolyElem>&);

	/*
	* Fills convex polygon
	*/
	void fillConvexPolygon(std::vector<PolyElem>&, Bresenham& bresenham, Grafika& graphics, bool);

	/*
	* Checks if polygon is convex and what is orientation of some polygon
	*/
	void checkConvexPolygon(std::vector<PolyElem>&, bool&, bool&);

	/*
	* Return true if point is in polygon, false if out
	*/
	bool pointPolygonRelation(glm::vec2&, std::vector<PolygonIRG::PolyElem>&, bool&);

	/*
	* Creates new polygon irg element
	*/
	PolygonIRG::PolyElem createNewPolygonElement(glm::vec2& last_vertex, glm::vec2& vertex, bool orientation);

};