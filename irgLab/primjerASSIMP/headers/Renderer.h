#pragma once

#include "Object.h"
#include "FPSManager.h"
#include <vector>

class Renderer {

private:
	void init();

public:
	std::vector<Object> objects;
	GLFWwindow* window = nullptr;
	int width, height;
	char* argv;

	Renderer(int mWidth, int mHeight, std::vector<Object>&, char*);

	~Renderer();




	void draw();


};