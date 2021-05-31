#pragma once

//delete later what you don't need
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class Curve {
private:
	std::vector<glm::vec3> vertices;
	std::vector<GLint> indices;
	Shader* shader;
	std::vector<glm::vec3> points;


public:
	Curve();
	~Curve();

	GLuint VAO[2]; //vao
	GLuint VBO[2]; //vbo
	GLuint EBO; //ebo


	std::vector<glm::vec3>& getVertices(); //return vertices
	std::vector<GLint>& getIndices(); //return indices

	void addVertex(glm::vec3&);

	void addPoint(glm::vec3&);

	std::vector<glm::vec3>& getPoints();

	void clear();

	void initGraphicBuffers();

	void deleteGraphicBuffers();

	void loadShader(char* path, char* naziv);

	Shader* getShader();
};