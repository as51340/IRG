#include "Curve.h"

Curve::Curve() {
};

Curve::~Curve() {
};

void Curve::initGraphicBuffers() {
	glGenVertexArrays(2, VAO); //jedan vao
	glGenBuffers(2, VBO); //jedan vao
	//glGenBuffers(1, &EBO); //jedan ebo

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

void Curve::deleteGraphicBuffers() {
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(2, VAO);
}

std::vector<glm::vec3>& Curve::getVertices() {
	return this->vertices;
}

std::vector<GLint>& Curve::getIndices() {
	return this->indices;
}


void Curve::addVertex(glm::vec3& ver) {
	this->vertices.push_back(ver);
}


void Curve::loadShader(char* path, char* naziv) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/') {
		pathVert.append("shaders/");
		pathFrag.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\') {
		pathVert.append("shaders\\");
		pathFrag.append("shaders\\");
	}
	else {
		std::cerr << "nepoznat format pozicije shadera";
		exit(1);
	}

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathFrag.append(naziv);
	pathFrag.append(".frag");

	shader = new Shader(pathVert.c_str(), pathFrag.c_str());
}

Shader* Curve::getShader() {
	return this->shader;
}

void Curve::addPoint(glm::vec3& point) {
	this->points.push_back(point);
}

void Curve::clear() {
	this->points.clear();
}

std::vector<glm::vec3>& Curve::getPoints() {
	return this->points;
}







