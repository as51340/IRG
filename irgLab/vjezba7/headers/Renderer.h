#pragma once

#include "Object.h"
#include "FPSManager.h"
#include <vector>
#include "Camera.h"
#include "Bezier.h"

class Renderer {

private:
	void init();

public:
	std::vector<Object> objects;
	GLFWwindow* window = nullptr;
	int width, height;
	char* argv;
	Curve curve, inter_curve;
	Bezier bezier;

	Camera camera;

	Renderer(int mWidth, int mHeight, std::vector<Object>&, char*, Camera&);

	~Renderer();

	void draw();


};