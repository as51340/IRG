#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <iostream>
#include <cstdlib>
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"

const int mWidth = 1280;
const int mHeight = 720;

void initMesh(Mesh& mesh, char* argv[], Material& material) {
	Assimp::Importer importer;
	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("\\resources"); //za linux pretvoriti u forwardslash
	std::string objPath(resPath);
	objPath.append("\\glava\\glava.obj"); //za linux pretvoriti u forwardslash

	const aiScene* scene = importer.ReadFile(objPath.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::cerr << importer.GetErrorString();
		exit(EXIT_FAILURE);
	}

	if (scene->HasMeshes()) {

		aiMesh* meshLocal = scene->mMeshes[0];
		//mesh.vertices = meshLocal->mVertices;
		//mesh.verticesNum = meshLocal->mVertices->Length();
		//mesh.textureCoords = meshLocal->mTextureCoords[0];
		//mesh.polygonsNum = meshLocal->mNumFaces;
		//mesh.polygons = meshLocal->mFaces;
		//mesh.normals = meshLocal->mNormals;
		//mesh.materials = scene->mMaterials;
		//mesh.resPath = resPath;

		mesh.resPath = resPath;



		for (int i = 0; i < meshLocal->mNumVertices; i++) {
			glm::vec3 vec = glm::vec3(meshLocal->mVertices[i].x, meshLocal->mVertices[i].y, meshLocal->mVertices[i].z);
			mesh.vertices.push_back(vec);

			/*glm::vec2 texture = glm::vec2(meshLocal->mTextureCoords[0][i].x, meshLocal->mTextureCoords[0][i].y);
			mesh.textureCoords.push_back(texture); */

			if (meshLocal->HasNormals()) {

				glm::vec3 normal = glm::vec3(meshLocal->mNormals[i].x, meshLocal->mNormals[i].y, meshLocal->mNormals[i].z);
				mesh.normals.push_back(normal);

			}
		}

		for (int i = 0; i < meshLocal->mNumFaces; i++) {
			for (int j = 0; j < meshLocal->mFaces[i].mNumIndices; j++) {

				int vertex = meshLocal->mFaces[i].mIndices[j];

				mesh.indices.push_back(vertex);
				//ADD materials sometime
			}
		}

		if (scene->HasMaterials()) {
			std::cout << "ucitana svojstva materijala" << std::endl;

			for (int i = 0; i < scene->mNumMaterials; i++) {

				aiString naziv;
				scene->mMaterials[i]->Get(AI_MATKEY_NAME, naziv);

				aiString texturePosition;
				int width, height, nrChannels;
				unsigned char* data;
				if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
					std::cout << texturePosition.C_Str() << std::endl;
					std::string texPath(resPath);


					texPath.append("\\glava\\");
					texPath.append(texturePosition.C_Str());
					data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
				}

				glm::vec3 ambientColor;
				aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;

				std::cout << "ambient: ";
				scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
				std::cout << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;

				glm::vec3 ambientMaterial = glm::vec3(ambientK.r, ambientK.g, ambientK.b); //new

				//std::cout << "diffuse: ";
				scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
				//std::cout << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;

				glm::vec3 diffMaterial = glm::vec3(diffuseK.r, diffuseK.g, diffuseK.b); //new


				std::cout << "specular: ";
				scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
				std::cout << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;

				glm::vec3 specMaterial = glm::vec3(specularK.r, specularK.g, specularK.b); //new


				std::cout << "emissive: ";
				scene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveK);
				std::cout << emissiveK.r << " " << emissiveK.g << " " << emissiveK.b << std::endl;

				std::cout << "- - - - - - - - - - - - -" << std::endl;


				std::cout << "____________________________________________________" << std::endl;


				material.amb = ambientMaterial;
				material.diff = diffMaterial;
				material.ref = specMaterial;
			}

		}
	}

}

int main(int argc, char* argv[]) {
	//std::cout << "First: " << argv[0] << std::endl;

	std::vector<Object> objects;
	Mesh mesh;
	Material material; //new
	initMesh(mesh, argv, material);

	Object obj(mesh, "shader1");
	objects.push_back(obj);

	glm::mat4 jed = glm::mat4(1.0);
	glm::vec3 trans = glm::vec3(0, 0, 0);
	jed = glm::translate(jed, trans);

	objects[0].transform.updateModel(jed);

	/*Object obj1(mesh, "shader1");
	objects.push_back(obj1);

	trans = glm::vec3(0, 0, 0.0);
	jed = glm::rotate(jed, 3.14159f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	jed = glm::translate(jed, trans);
	objects[1].transform.updateModel(jed);*/

	glm::vec3 eye = glm::vec3(1, 3, 3);
	glm::vec3 target = glm::vec3(0, 0, 0);
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);
	Camera camera(eye, target, cameraUp);

	Light light;
	light.position = glm::vec3(3, 4, 5);
	light.ii = glm::vec3(0.0, 0.8, 0.9);
	light.ia = glm::vec3(0.2, 0.2, 0.2);

	Renderer* renderer = new Renderer(mWidth, mHeight, objects, argv[0], camera, material, light); //renderer ce mi ga initat

	delete renderer;

	return EXIT_SUCCESS;
}