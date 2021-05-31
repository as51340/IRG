#include "Object.h"

Object::Object(Mesh& mesh, char* shaderName) {
	this->mesh = mesh;
	this->shaderName = shaderName;
	//mesh.printVertices();
};


void Object::transformObject() {
	Transform transform;
	std::pair<glm::vec3, glm::vec3> boxs = this->mesh.getBoundingBox();
	/*std::cout << "First " << boxs.second.x << " second " << boxs.first.x << std::endl;
	std::cout << "First " << boxs.second.y << " second " << boxs.first.y << std::endl;
	std::cout << "First " << boxs.second.z << " second " << boxs.first.z << std::endl;*/

	//mesh.printVertices();

	float x_as = (boxs.first.x + boxs.second.x) * 0.5;
	//std::cout << "X_as: " << x_as << std::endl;
	float y_as = (boxs.first.y + boxs.second.y) * 0.5;
	//std::cout << "Y_as: " << y_as << std::endl;
	float z_as = (boxs.first.z + boxs.second.z) * 0.5;
	//std::cout << "Z_as: " << z_as << std::endl;

	glm::mat4 trans_mat = transform.createTransformMatrix(-x_as, -y_as, -z_as);
	//std::cout << glm::to_string(trans_mat) << std::endl;
	this->mesh.applyTransform(trans_mat);

	//mesh.printVertices();


	float M = boxs.second.x - boxs.first.x;
	float yM = boxs.second.y - boxs.first.y;
	float zM = boxs.second.z - boxs.first.z;
	if (yM > M) M = yM;
	if (zM > M) M = zM;
	glm::mat4 scaling = transform.createScalingMatrix(M);
	this->mesh.applyTransform(scaling);



}


void Object::loadShader(char* path, char* naziv) {
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

	sjencar = new Shader(pathVert.c_str(), pathFrag.c_str());
}
