#include "Renderer.h"

Renderer::Renderer(int mWidth, int mHeight, std::vector<Object>& objects, char* argv) : width(mWidth), height(mHeight), argv(argv) {
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora


	//later change to more objects

	for (int i = 0; i < objects.size(); i++) {
		objects[i].loadShader(argv, objects[i].shaderName);
		objects[i].transformObject();
		//objects[i].mesh.printVertices();
		objects[i].mesh.initGraphicBuffers();
	}

	while (glfwWindowShouldClose(window) == false) {

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		for (int i = 0; i < objects.size(); i++) {
			//std::cout << "Size: " << objects.size() << std::endl;
			Object obj = objects[i];
			GLint uniformQuadColor = glGetUniformLocation(obj.sjencar->ID, "u_color");
			glUseProgram(obj.sjencar->ID);
			glBindVertexArray(obj.mesh.VAO);
			glUniform3f(uniformQuadColor, 1.0, 1.0, 1.0); //white
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
