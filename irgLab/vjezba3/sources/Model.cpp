#include "Model.h"

Model::Model() {
	/*colorVertices.push_back(glm::vec3(-0.95, 0.95, 0)); //upper left
	colorVertices.push_back(glm::vec3(-0.95, 0.85, 0)); //bottom left
	colorVertices.push_back(glm::vec3(-0.85, 0.95, 0)); //upper right
	colorVertices.push_back(glm::vec3(-0.85, 0.85, 0)); //bottom right*/
	currentColor = glm::vec3(1.0, 1.0, 1.0); //init white
	colorVertices.push_back(glm::vec3(mapToGL(glm::vec2(10, 10)), 0)); //upper left
	colorVertices.push_back(glm::vec3(mapToGL(glm::vec2(10, 40)), 0)); //upper left
	colorVertices.push_back(glm::vec3(mapToGL(glm::vec2(40, 10)), 0)); //upper left
	colorVertices.push_back(glm::vec3(mapToGL(glm::vec2(40, 40)), 0)); //upper left
};

Model::~Model() {};

glm::vec2 Model::cursorPosition(0, 0);

void Model::changeCurrColorComponentSelected(char newColor) { this->currColorComponentSelected = newColor; }

//think how to map this to 0 to 1 in open GL
void Model::increaseColorComponent() {
	if (currColorComponentSelected == 'R') {
		currentColor.x += changeColorComponentRate;
		if (currentColor.x > 1) currentColor.x = 1;
	}
	else if (currColorComponentSelected == 'G') {
		currentColor.y += changeColorComponentRate;
		if (currentColor.y > 1) currentColor.y = 1;
	}
	else if (currColorComponentSelected == 'B') {
		currentColor.z += changeColorComponentRate;
		if (currentColor.z > 1) currentColor.z = 1;
	}

}

void Model::decreaseColorComponent() {
	//std::cout << currColorComponentSelected << std::endl;
	if (currColorComponentSelected == 'R') {
		currentColor.x -= changeColorComponentRate;
		if (currentColor.x < 0) currentColor.x = 0;
		//std::cout << "Novi r " << currentColor.x << std::endl;
	}
	else if (currColorComponentSelected == 'G') {
		currentColor.y -= changeColorComponentRate;
		if (currentColor.y < 0) currentColor.y = 0;
		//std::cout << "Novi g " << currentColor.y << std::endl;
	}
	else if (currColorComponentSelected == 'B') {
		currentColor.z -= changeColorComponentRate;
		if (currentColor.z < 0) currentColor.z = 0;
		//std::cout << "Novi b " << currentColor.z << std::endl;
	}
}

glm::vec2 Model::mapToGL(glm::vec2& world) {
	float gl_x = 2 * ((float)world.x / width) - 1;
	float gl_y = -2 * ((float)world.y / height) + 1;
	//std::cout << "Gl_x: " << gl_x << " Gl_y " << gl_y << std::endl;
	return glm::vec2(gl_x, gl_y);
}
