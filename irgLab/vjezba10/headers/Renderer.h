#pragma once

#include "Object.h"
#include "FPSManager.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Renderer {

private:
	void init();

public:
	std::vector<Object> objects;
	GLFWwindow* window = nullptr;
	int width, height;
	char* argv;

	Camera camera;
	Light light;
	Material material;

	Renderer(int mWidth, int mHeight, std::vector<Object>&, char*, Camera&, Material&, Light&);

	Shader* loadShader(char* path, char* naziv);

	~Renderer();


};