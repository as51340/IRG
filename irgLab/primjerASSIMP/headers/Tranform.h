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

public:
	Transform();

	~Transform();

	glm::mat4 createTransformMatrix(float x, float y, float z);

	glm::mat4 createScalingMatrix(float);

};


