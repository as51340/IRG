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



class Material {
public:
	glm::vec3 amb, diff, ref;
};