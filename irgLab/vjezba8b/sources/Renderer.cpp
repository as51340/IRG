#include "Renderer.h"
#include "Camera.h"
#include <glm\gtc\type_ptr.hpp>

Renderer::Renderer(int mWidth, int mHeight, std::vector<Object>& objects, char* argv, Camera& camera, Material& material, Light& light)
	: width(mWidth), height(mHeight), argv(argv), camera(camera) {
	this->light = light;
	this->material = material;
	this->objects = objects;
	init();
};

Renderer::~Renderer() {
	objects[0].mesh.deleteGraphicBuffers();
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i].sjencar;
	}
	glfwTerminate();
};

void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

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


	//FOR removing back polygons
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//later change to more objects

	for (int i = 0; i < objects.size(); i++) {
		objects[i].loadShader(argv, objects[i].shaderName);
		objects[i].transformObject();
		objects[i].mesh.initGraphicBuffers();
		//std::cout << glm::to_string(objects[i].transform.getModelMatrix()) << std::endl;
	}

	/*glm::vec3 center;

	glm::vec3 v1 = light.position - center;
	glm::vec3 v2 = eye - center;
	float m1 = glm::dot(v1, out_normal);
	if (m1 < 0)
	{
		m1 = 0;
	}
	glm::vec3 h = (v1 + v2) / 2;

	float m2 = dot(v2, h);
	if (m2 < 0) m2 = 0;

	float ir = ia.x * amb.x + ii.x * (diff.x * m1 + ref.x * pow(m2, 10));
	float ig = ia.y * amb.y + ii.y * (diff.y * m1 + ref.y * pow(m2, 10));
	float ib = ia.z * amb.z + ii.z * (diff.z * m1 + ref.z * pow(m2, 10));*/

	//std::cout << material.amb.x << " " << material.amb.y << " " << material.amb.z << std::endl;

	while (glfwWindowShouldClose(window) == false) {

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		for (int i = 0; i < objects.size(); i++) {
			//std::cout << "Size: " << objects.size() << std::endl;
			Object obj = objects[i];
			glUseProgram(obj.sjencar->ID);
			glBindVertexArray(obj.mesh.VAO);


			GLint uniformEye = glGetUniformLocation(obj.sjencar->ID, "eye"); //for fragment shader
			GLint modelUniform = glGetUniformLocation(obj.sjencar->ID, "model");
			GLint viewUniform = glGetUniformLocation(obj.sjencar->ID, "view");
			GLint projectionUniform = glGetUniformLocation(obj.sjencar->ID, "projection");
			GLint lightUniform = glGetUniformLocation(obj.sjencar->ID, "light_position");
			GLint ambUniform = glGetUniformLocation(obj.sjencar->ID, "amb");
			GLint diffUniform = glGetUniformLocation(obj.sjencar->ID, "diff");
			GLint refUniform = glGetUniformLocation(obj.sjencar->ID, "ref");
			GLint iiUniform = glGetUniformLocation(obj.sjencar->ID, "ii");
			GLint iaUniform = glGetUniformLocation(obj.sjencar->ID, "ia");


			glUniform3f(uniformEye, camera.eye.x, camera.eye.y, camera.eye.z);
			glUniform3f(lightUniform, light.position.x, light.position.y, light.position.z);
			glUniform3f(ambUniform, material.amb.x, material.amb.y, material.amb.z);
			glUniform3f(diffUniform, material.diff.x, material.diff.y, material.diff.z);
			glUniform3f(refUniform, material.ref.x, material.ref.y, material.ref.z);
			glUniform3f(iiUniform, light.ii.x, light.ii.y, light.ii.z);
			glUniform3f(iaUniform, light.ia.x, light.ia.y, light.ia.z);


			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(obj.transform.getModelMatrix()));
			glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
			glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(camera.getPerspectiveMatrix(1920, 1080)));


			glDrawElements(GL_TRIANGLES, obj.mesh.indices.size(), GL_UNSIGNED_INT, 0);


			glBindVertexArray(0); //otkvaèi
			//std::cout << glGetError() << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}


	}
}
