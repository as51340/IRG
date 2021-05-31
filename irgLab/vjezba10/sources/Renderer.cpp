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

/*
* For loading shadow mapping shaders
*/
Shader* Renderer::loadShader(char* path, char* naziv) {
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

	FPSManager FPSManagerObject(window, 60, 1.0, "4. lab. vjezba");
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
	}

	//TEXTURE BINDING
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, objects[0].mesh.textWidth, objects[0].mesh.textHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, objects[0].mesh.data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glGenerateMipmap(GL_TEXTURE_2D);


	unsigned int mShadowMapID, mFramebufferID;
	//From light source setup
	glGenTextures(1, &mShadowMapID);
	glBindTexture(GL_TEXTURE_2D, mShadowMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); //prije je bilo na 1440 1080
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//attach depth texture as FBO's depth buffer
	glGenFramebuffers(1, &mFramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader* lightShader = loadShader(argv, "simpleLightShader");
	//Shader* debugShader = loadShader(argv, "debugDepthMapShader");

	// Light variables
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = -0.5f, far_plane = 110.5f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene

	float sumTime = 0;

	while (glfwWindowShouldClose(window) == false) {

		lightProjection = glm::frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 50.0);
		lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0)); // read again
		lightSpaceMatrix = lightProjection * lightView;

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//First render scene from light's perspective
		//Prepare light shader and depth map texture
		for (int i = 0; i < objects.size(); i++) {
			Object obj = objects[i];
			glUseProgram(lightShader->ID); //pogledaj sta jos treba slat
			glBindVertexArray(obj.mesh.VAO);

			GLint modelUniform = glGetUniformLocation(lightShader->ID, "model");
			GLint lightSpaceMatrixUniform = glGetUniformLocation(lightShader->ID, "lightSpaceMatrix"); //new for shadow mapping
			GLint shadowMapLoc = glGetUniformLocation(obj.sjencar->ID, "shadowMap");

			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(obj.transform.getModelMatrix()));
			glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix)); //light space matrix
			glUniform1i(shadowMapLoc, 0); // Texture unit 0 is for shadow map.

			glActiveTexture(GL_TEXTURE0); //need
			glBindTexture(GL_TEXTURE_2D, mShadowMapID); //need
			glDrawElements(GL_TRIANGLES, obj.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Clear for second drawing
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear again for light source shader
		//Second scene drawing

		for (int i = 0; i < objects.size(); i++) {
			Object obj = objects[i];
			glUseProgram(obj.sjencar->ID);
			glBindVertexArray(obj.mesh.VAO);

			GLint uniformEye = glGetUniformLocation(obj.sjencar->ID, "eye"); //for fragment shader
			GLint modelUniform = glGetUniformLocation(obj.sjencar->ID, "model");
			GLint viewUniform = glGetUniformLocation(obj.sjencar->ID, "view");
			GLint projectionUniform = glGetUniformLocation(obj.sjencar->ID, "projection");
			GLint lightSpaceMatrixUniform = glGetUniformLocation(obj.sjencar->ID, "lightSpaceMatrix"); //new for shadow mapping
			GLint lightUniform = glGetUniformLocation(obj.sjencar->ID, "light_position");
			GLint ambUniform = glGetUniformLocation(obj.sjencar->ID, "amb");
			GLint diffUniform = glGetUniformLocation(obj.sjencar->ID, "diff");
			GLint refUniform = glGetUniformLocation(obj.sjencar->ID, "ref");
			GLint iiUniform = glGetUniformLocation(obj.sjencar->ID, "ii");
			GLint iaUniform = glGetUniformLocation(obj.sjencar->ID, "ia");
			GLint normalMapLoc = glGetUniformLocation(obj.sjencar->ID, "ourTexture");
			GLint shadowMapLoc = glGetUniformLocation(obj.sjencar->ID, "shadowMap");
			glUniform1i(normalMapLoc, 0); // Texture unit 0 is for normal maps.
			glUniform1i(shadowMapLoc, 1); // Texture unit 2 is for shadow maps

			//Send uniform variables for Phong's lighting
			glUniform3f(uniformEye, camera.eye.x, camera.eye.y, camera.eye.z);
			glUniform3f(lightUniform, light.position.x, light.position.y, light.position.z);
			glUniform3f(ambUniform, material.amb.x, material.amb.y, material.amb.z);
			glUniform3f(diffUniform, material.diff.x, material.diff.y, material.diff.z);
			glUniform3f(refUniform, material.ref.x, material.ref.y, material.ref.z);
			glUniform3f(iiUniform, light.ii.x, light.ii.y, light.ii.z);
			glUniform3f(iaUniform, light.ia.x, light.ia.y, light.ia.z);

			glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(obj.transform.getModelMatrix())); //model matrix
			glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix())); //view matrix
			glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(camera.getPerspectiveMatrix(1920, 1080))); //projection matrix
			glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix)); //light space matrix

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture); //this is objects texture
			glActiveTexture(GL_TEXTURE0 + 1); //depth map texture
			glBindTexture(GL_TEXTURE_2D, mShadowMapID); //second texture
			glDrawElements(GL_TRIANGLES, obj.mesh.indices.size(), GL_UNSIGNED_INT, 0); //draw with ebo setup
			glBindVertexArray(0); //otkvaèi
			//std::cout << glGetError() << std::endl;
			light.position.x = glm::sin(sumTime * 0.1);
			sumTime += deltaTime;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}




