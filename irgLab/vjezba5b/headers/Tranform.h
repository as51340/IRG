#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
//#define STB_IMAGE_IMPLEMENTATION, ne radi ni ako je includano prije assimpa
#include "stb_image.h"
#include<iostream>
#include <glm/gtx/string_cast.hpp>


class Transform {
private:

	glm::mat4 model = glm::mat4(1);

public:
	Transform();

	~Transform();

	glm::mat4 createTransformMatrix(float x, float y, float z);

	glm::mat4 createScalingMatrix(float);

	virtual glm::mat4 getViewMatrix();

	glm::mat4 getModelMatrix();

	void updateModel(glm::mat4&);



	static glm::mat4 translate3D(glm::vec3& translateVector);

	static glm::mat4 scale3D(glm::vec3& scale);

	static glm::mat4 lookAtMatrix(glm::vec3&, glm::vec3&, glm::vec3&);

	static glm::mat4 frustum(double, double, double, double, double, double);


};


