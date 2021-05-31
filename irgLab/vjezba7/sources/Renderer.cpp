#include "Renderer.h"
#include "Camera.h"
#include "Bezier.h"
#include <glm\gtc\type_ptr.hpp>

bool animation = false;
float t = 0;
float change = 0.0002;

Renderer::Renderer(int mWidth, int mHeight, std::vector<Object>& objects, char* argv, Camera& camera)
	: width(mWidth), height(mHeight), argv(argv), camera(camera) {
	this->objects = objects;
	init();
};

Renderer::~Renderer() {
	objects[0].mesh.deleteGraphicBuffers();
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i].sjencar;
	}
	curve.deleteGraphicBuffers();
	glfwTerminate();
};

void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && animation == false) {
		Renderer* rend = (Renderer*)glfwGetWindowUserPointer(window); //get pointer
		std::cout << rend->camera.eye.x << " " << rend->camera.eye.y << " " << rend->camera.eye.z << "\n";
		rend->curve.addVertex(rend->camera.eye);
		if (rend->curve.getVertices().size() > 1) {
			rend->curve.clear();
			rend->bezier.draw_bezier(rend->curve, 1000); //first curve solved

			rend->curve.initGraphicBuffers();

			if (rend->curve.getVertices().size() < 4) {
				return;
			}

			std::vector<glm::vec3> newVertices;
			for (int i = 0; i < 4; i++) {
				newVertices.push_back(rend->curve.getVertices()[rend->curve.getVertices().size() - i - 1]);
			}

			std::vector<glm::vec3> points = rend->bezier.interPoints(newVertices); //get actual vertices for interpolation curve
			rend->inter_curve.clear();
			rend->inter_curve.getVertices().clear();
			for (int i = 0; i < points.size(); i++) {
				rend->inter_curve.addVertex(points[i]);
			}
			rend->bezier.draw_bezier(rend->inter_curve, 1000);

			rend->inter_curve.initGraphicBuffers();
		}
	}
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Renderer* rend = (Renderer*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.direction;
			rend->camera.eye += delta;
		}
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.cameraX;
			rend->camera.eye += delta;
		}
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.direction;
			rend->camera.eye -= delta;
		}
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.cameraX;
			rend->camera.eye -= delta;
		}
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.cameraY;
			rend->camera.eye += delta;
		}
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		for (int i = 0; i < rend->objects.size(); i++) {
			glm::vec3 delta = rend->camera.getCameraSpeed() * rend->camera.cameraY;
			rend->camera.eye -= delta;
		}
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (animation == false) {
			std::cout << "Animation started!\n";
			animation = true;
		}
		else {
			std::cout << "Animation finished!\n";
			animation = false;
		}
		t = 0;
		change = 0.0002;
	}
}

Shader* loadShader(char* path, char* naziv) {
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

	return new Shader(pathVert.c_str(), pathFrag.c_str());
}

void Renderer::init() {
	glfwInit();
	gladLoadGL();
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	//za monitor i share salje 2 nullptr-a
	window = glfwCreateWindow(width, height, "3. lab. vjezba", nullptr, nullptr);
	// provjeri je li se uspio napraviti prozor
	if (window == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	// dohvati sve dostupne OpenGL funkcije
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	glEnable(GL_DEPTH_TEST); //ukljuci z spremnik (prikazuju se oni fragmenti koji su najblizi promatracu)
	glDepthFunc(GL_LESS);
	glClearColor(0.15, 0.1, 0.1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira
	glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

	FPSManager FPSManagerObject(window, 60, 1.0, "3. lab. vjezba");
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	//later change to more objects

	for (int i = 0; i < objects.size(); i++) {
		objects[i].loadShader(argv, objects[i].shaderName);
		objects[i].transformObject();
		objects[i].mesh.initGraphicBuffers();
		//std::cout << glm::to_string(objects[i].transform.getModelMatrix()) << std::endl;
	}

	/*glm::vec3 v1 = glm::vec3(-0.15, -0.05, 0.25);
	glm::vec3 v2 = glm::vec3(0.15, 0.05, 0.25);
	glm::vec3 v3 = glm::vec3(0.15, 0.05, -0.25);
	glm::vec3 v4 = glm::vec3(-0.15, -0.05, -0.25);
	this->curve.addVertex(v1);
	this->curve.addVertex(v2);
	this->curve.addVertex(v3);
	this->curve.addVertex(v4);

	bezier.draw_bezier(curve, 1000); //first curve solved

	std::vector<glm::vec3> points = bezier.interPoints(curve.getVertices()); //get actual vertices for interpolation curve
	for (int i = 0; i < points.size(); i++) {
		inter_curve.addVertex(points[i]);
	}
	bezier.draw_bezier(inter_curve, 1000);*/


	curve.loadShader(argv, "shader1");
	inter_curve.loadShader(argv, "shader3");
	curve.initGraphicBuffers();
	inter_curve.initGraphicBuffers();
	GLint uniformQuadColor = glGetUniformLocation(curve.getShader()->ID, "u_color");
	GLint uniformQuadColor2 = glGetUniformLocation(inter_curve.getShader()->ID, "u_color");



	/*for (int i = 0; i < curve.getPoints().size(); i++) {
		std::cout << curve.getPoints()[i].x << " " << curve.getPoints()[i].y << " " << curve.getPoints()[i].z << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < curve.getVertices().size(); i++) {
		std::cout << curve.getVertices()[i].x << " " << curve.getVertices()[i].y << " " << curve.getVertices()[i].z << std::endl;
	}*/

	while (glfwWindowShouldClose(window) == false) {

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (animation == false) { //new
			if (curve.getVertices().size() >= 1) {

				glUseProgram(curve.getShader()->ID);
				glUniform3f(uniformQuadColor, 255, 0, 0); //red
				//glViewport(0, height / 2, width / 3, height / 2);
				glBindVertexArray(curve.VAO[0]);
				glDrawArrays(GL_LINE_STRIP, 0, curve.getPoints().size());

				glUniform3f(uniformQuadColor, 0, 0, 255); //blue
				//glViewport(0, height / 2, width / 3, height / 2);
				glBindVertexArray(curve.VAO[1]);
				glDrawArrays(GL_LINE_STRIP, 0, curve.getVertices().size());
				glBindVertexArray(0); //otkvaèi

				glUseProgram(inter_curve.getShader()->ID);
				glUniform3f(uniformQuadColor2, 0, 255, 0); //white
				//glViewport(0, height / 2, width / 3, height / 2);
				glBindVertexArray(inter_curve.VAO[0]);
				glDrawArrays(GL_LINE_STRIP, 0, inter_curve.getPoints().size());
				glBindVertexArray(0); //otkvaèi

				//this->curve.clear();
			}
		}
		else {

			camera.eye = bezier.getPoint(t, inter_curve.getVertices());

			for (int i = 0; i < objects.size(); i++) {
				//std::cout << "Size: " << objects.size() << std::endl;
				Object obj = objects[i];
				glUseProgram(obj.sjencar->ID);
				glBindVertexArray(obj.mesh.VAO);


				GLint uniformQuadColor = glGetUniformLocation(obj.sjencar->ID, "u_color");
				GLint modelUniform = glGetUniformLocation(obj.sjencar->ID, "model");
				GLint viewUniform = glGetUniformLocation(obj.sjencar->ID, "view");
				GLint projectionUniform = glGetUniformLocation(obj.sjencar->ID, "projection");



				glUniform3f(uniformQuadColor, 1.0, 1.0, 1.0); //white
				glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(obj.transform.getModelMatrix()));
				glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
				glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(camera.getPerspectiveMatrix(1920, 1080)));
				glDrawElements(GL_LINE_LOOP, obj.mesh.indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0); //otkvaèi
			}
			t += change;
			if (t > 1 || t < 0) {
				change *= -1;
			}
		}



		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}


	}
}
