#include "Mesh.h"

void Mesh::printVertices() {
	std::cout << "Printing vertices: \n";
	for (int i = 0; i < vertices.size(); i++) {
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}
	std::cout << "\n";;
}

void Mesh::printTextureCoord() {
	std::cout << "Printing texture: \n";
	int size = vertices.size();
	for (int i = 0; i < 10; i++) {
		std::cout << textureCoords[i].x << " " << textureCoords[i].y << std::endl;
	}
	std::cout << "\n";;
}

void Mesh::printPolygons() {
	std::cout << "Printing indices: \n";
	int indicesSize = indices.size();
	for (int i = 0; i < 10; i++) {
		std::cout << indices[i] << " ";
	}
	std::cout << "\n";;
}

void Mesh::printNormals() {
	std::cout << "Printing normals: \n";
	int indicesSize = indices.size();
	for (int i = 0; i < 10; i++) {
		std::cout << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
	}
	std::cout << "\n";;
}

void Mesh::printMaterials() {
	/*std::cout << "Printing materials: \n";
	for (int i = 0; i < mNumMaterials; i++) {
		aiString naziv;
		materials[i]->Get(AI_MATKEY_NAME, naziv);
		std::cout << naziv.C_Str() << std::endl;
		aiString texturePosition;
		int width, height, nrChannels;
		unsigned char* data;
		if (AI_SUCCESS == materials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
			std::cout << texturePosition.C_Str() << std::endl;
			std::string texPath(resPath);


			texPath.append("\\glava\\");
			texPath.append(texturePosition.C_Str());
			data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
		}
		glm::vec3 ambientColor;
		aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;

		std::cout << "ambient: ";
		materials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
		std::cout << ambientK.r << " " << ambientK.g << " " << ambientK.b << std::endl;

		std::cout << "diffuse: ";
		materials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
		std::cout << diffuseK.r << " " << diffuseK.g << " " << diffuseK.b << std::endl;

		std::cout << "specular: ";
		materials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);
		std::cout << specularK.r << " " << specularK.g << " " << specularK.b << std::endl;


		std::cout << "emissive: ";
		materials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveK);
		std::cout << emissiveK.r << " " << emissiveK.g << " " << emissiveK.b << std::endl;

		std::cout << "- - - - - - - - - - - - -" << std::endl;


		std::cout << "____________________________________________________" << std::endl;
	}
	std::cout << "\n";;*/
}

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox() {
	glm::vec3 min(vertices[0].x, vertices[0].y, vertices[0].z), max(vertices[0].x, vertices[0].y, vertices[0].z);
	int size = vertices.size();
	for (int i = 1; i < size; i++) {
		//std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
		if (vertices[i].x < min.x) min.x = vertices[i].x;
		if (vertices[i].y < min.y) min.y = vertices[i].y;
		if (vertices[i].z < min.z) min.z = vertices[i].z;

		if (vertices[i].x > max.x) max.x = vertices[i].x;
		if (vertices[i].y > max.y) max.y = vertices[i].y;
		if (vertices[i].z > max.z) max.z = vertices[i].z;
	}
	//std::cout << max.x << " " << min.y << " " << min.z << std::endl;
	//std::cout << max.x << " " << max.y << " " << max.z << std::endl;
	return std::make_pair(min, max);
}

void Mesh::initGraphicBuffers() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW); //jos veci upitnik

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(2);


	glBindVertexArray(0); //otkvaèi, dobra programerska praksa


}

void Mesh::deleteGraphicBuffers() {
	glDeleteBuffers(3, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::applyTransform(glm::mat4& transf) {
	int size = vertices.size();
	//std::cout << glm::to_string(transf);
	for (int i = 0; i < size; i++) {
		//std::cout << "before " << glm::to_string(vertices[i]) << std::endl;
		glm::vec4 temp = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, (float)1.0);
		glm::vec4 sol = transf * temp;
		//std::cout << "After " << glm::to_string(sol) << std::endl;
		vertices[i].x = sol.x;
		vertices[i].y = sol.y;
		vertices[i].z = sol.z;
	}

	//update buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW); //jos veci upitnik


}
