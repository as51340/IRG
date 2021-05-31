#pragma once

//delete later what you don't need
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
//#define STB_IMAGE_IMPLEMENTATION, ne radi ni ako je includano prije assimpa
#include "stb_image.h"
#include<iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Mesh {
public:

	aiScene* scene;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<GLint> indices;
	std::vector<glm::vec3> normals;

	//unsigned int mNumMaterials = 0;
	//aiMaterial** materials = nullptr;
	std::string resPath;

	//funny part
	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;
	unsigned char* data;

	void initGraphicBuffers();
	void deleteGraphicBuffers();

	int textWidth, textHeight;

	void printVertices();
	void printTextureCoord();
	void printPolygons();
	void printNormals();
	void printMaterials();

	void applyTransform(glm::mat4&);

	//returns min/max coords
	std::pair<glm::vec3, glm::vec3> getBoundingBox();


};
