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

const int mWidth = 1280;
const int mHeight = 720;

void initMesh(Mesh& mesh, char* argv[]) {
	Assimp::Importer importer;
	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("\\resources"); //za linux pretvoriti u forwardslash
	std::string objPath(resPath);
	objPath.append("\\glava\\kocka.obj"); //za linux pretvoriti u forwardslash

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
			mesh.textureCoords.push_back(texture);

			glm::vec3 normal = glm::vec3(meshLocal->mNormals[i].x, meshLocal->mNormals[i].y, meshLocal->mNormals[i].z);
			mesh.normals.push_back(normal);*/
		}

		for (int i = 0; i < meshLocal->mNumFaces; i++) {
			for (int j = 0; j < meshLocal->mFaces[i].mNumIndices; j++) {

				int vertex = meshLocal->mFaces[i].mIndices[j];

				mesh.indices.push_back(vertex);
				//ADD materials sometime
			}
		}
	}

}



int main(int argc, char* argv[]) {
	//std::cout << "First: " << argv[0] << std::endl;

	std::vector<Object> objects;
	Mesh mesh;
	initMesh(mesh, argv);
	//std::cout << mesh.resPath << " " << mesh.vertices.size() << "n" << mesh.indices.size() << std::endl;

	//mesh.printVertices();

	Object obj(mesh, "shader1");
	objects.push_back(obj);

	glm::mat4 jed = glm::mat4(1.0);
	glm::vec3 trans = glm::vec3(0, 0, 0.75);
	jed = glm::translate(jed, trans);
	//std::cout << glm::to_string(jed) << std::endl;

	objects[0].transform.updateModel(jed);
	//std::cout << glm::to_string(obj.transform.getModelMatrix()) << std::endl;

	Object obj1(mesh, "shader1");
	objects.push_back(obj1);

	trans = glm::vec3(0, 0, -1.0);
	jed = glm::rotate(jed, 3.14159f / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	jed = glm::translate(jed, trans);
	objects[1].transform.updateModel(jed);

	glm::vec3 eye = glm::vec3(3, 4, 1);
	glm::vec3 target = glm::vec3(0, 0, 0);
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);
	Camera camera(eye, target, cameraUp);

	Renderer* renderer = new Renderer(mWidth, mHeight, objects, argv[0], camera); //renderer ce mi ga initat

	delete renderer;

	return EXIT_SUCCESS;
}