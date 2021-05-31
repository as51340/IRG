#include "Renderer.h"
#include "Camera.h"
#include <glm\gtc\type_ptr.hpp>

Renderer::Renderer(int mWidth, int mHeight, std::vector<Object>& objects, char* argv, Camera& camera)
	: width(mWidth), height(mHeight), argv(argv), camera(camera) {
	this->objects = objects;
	init();
};

Renderer::~Renderer() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i].sjencar;
		objects[i].mesh.deleteGraphicBuffers();
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

	//later change to more objects

	for (int i = 0; i < objects.size(); i++) {
		objects[i].loadShader(argv, objects[i].shaderName);
		objects[i].transformObject();
		objects[i].mesh.initGraphicBuffers();
		//std::cout << glm::to_string(objects[i].transform.getModelMatrix()) << std::endl;
	}
	/*glm::mat4 one = glm::mat4(1.0);

	glm::mat4 view = Transform::lookAtMatrix(camera.eye, camera.target, camera.cameraUp);
	glm::mat4 proj = Transform::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);

	objects[0].mesh.applyTransform(objects[0].transform.getModelMatrix());

	objects[0].mesh.printVertices();

	for (int i = 0; i < objects[0].mesh.vertices.size(); i++) {
		glm::vec4 sol = glm::vec4(objects[0].mesh.vertices[i], 1.0);
		sol = proj * view * sol;
		objects[0].mesh.vertices[i].x = sol.x;
		objects[0].mesh.vertices[i].y = sol.y;
		objects[0].mesh.vertices[i].z = sol.z;
	}
	objects[0].mesh.printVertices();

	//objects[0].mesh.applyTransform(view * objects[0].transform.getModelMatrix());
	*/

	while (glfwWindowShouldClose(window) == false) {

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}


	}
}
