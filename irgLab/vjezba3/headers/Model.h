#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>



class Model {

public:

	glm::vec3 currentColor;
	int width = 1000, height = 1000;
	char currColorComponentSelected = '0'; // R, G, B
	float changeColorComponentRate = 0.05;
	std::vector<glm::vec3> colorVertices;
	std::vector<GLint> indices;
	//std::vector<glm::vec3> vertices; //we'll see how to organize that
	std::vector<glm::vec3> triangleColors;
	std::vector<glm::vec3> vertices;
	bool updateable = true;
	std::vector<glm::vec3> bridges;

	Model();
	~Model();

	static glm::vec2 cursorPosition;

	void changeCurrColorComponentSelected(char);
	void increaseColorComponent();
	void decreaseColorComponent();

	glm::vec2 mapToGL(glm::vec2&);


};