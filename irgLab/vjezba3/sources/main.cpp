// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "FPSManager.h"
#include "Model.h"
// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>


Shader* sjencar[2];
Model* model = new Model;
GLuint VAO[3]; //trebam 3 vao objekta jedan za onaj kvadratic jedan za linije jedan za trokute
GLuint VBO[4];
GLuint EBO;

//malo je nespretno napravljeno jer ne koristimo c++17, a treba podrzati i windows i linux,
//slobodno pozivajte new Shader(...); direktno. Naziv je kod shader!!. 
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



//funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju povezati pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
	model->width = Width;
	model->height = Height;

	glViewport(0, 0, model->width, model->height);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (!model->updateable) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::vec3 transf(model->mapToGL(model->cursorPosition), 0);
		//std::cout << "x " << transf.x << " y " << transf.y << " z " << transf.z << std::endl;
		model->vertices.push_back(transf);
		model->bridges.push_back(transf);
		model->triangleColors.push_back(model->currentColor);
		int vertSize = model->vertices.size();
		if (vertSize >= 3) {
			model->indices.push_back(vertSize - 3);
			model->indices.push_back(vertSize - 2);
			model->indices.push_back(vertSize - 1);
			model->bridges.erase(model->bridges.begin());
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), model->vertices.data(), GL_STATIC_DRAW); //tu ide velièina buffera
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, model->triangleColors.size() * sizeof(glm::vec3), model->triangleColors.data(), GL_STATIC_DRAW); //tu ide velièina buffera
		//EBO za trokute
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempIndexes.size() * sizeof(GLint), tempIndexes.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices.size() * sizeof(GLint), model->indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, model->bridges.size() * sizeof(glm::vec3), model->bridges.data(), GL_STATIC_DRAW); //tu ide velièina buffera
	}
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!model->updateable) return;
	//model->cursorPosition = model->mapToGL(glm::vec2(xpos, ypos));
	model->cursorPosition = glm::vec2(xpos, ypos);
	/*if (model->bridges.size() >= 1 && model->bridges.size() < 3) {
		model->bridges.insert(model->bridges.begin() + 1, glm::vec3(model->mapToGL(model->cursorPosition), 0));
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, model->bridges.size() * sizeof(glm::vec3), model->bridges.data(), GL_STATIC_DRAW); //tu ide velièina buffera
	}*/

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!model->updateable) return;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		model->changeCurrColorComponentSelected('R');
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		model->changeCurrColorComponentSelected('G');
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		model->changeCurrColorComponentSelected('B');
	}
	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		model->increaseColorComponent();
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		model->decreaseColorComponent();
		//std::cout << model->currentColor.x << " " << model->currentColor.y << " " << model->currentColor.z << std::endl;
	}
}

int main(int argc, char* argv[]) {
	std::cout << "First: " << argv[0] << std::endl;
	/*********************************************************************************************/
	//postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
	GLFWwindow* window;
	glfwInit();
	gladLoadGL();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	//za monitor i share salje 2 nullptr-a
	window = glfwCreateWindow(model->width, model->height, "3. lab. vjezba", nullptr, nullptr);
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

	//glEnable(GL_CULL_FACE); //ukljuci uklanjanje straznjih poligona -- za ovaj primjer je iskljuceno
	//glCullFace(GL_BACK); 

	glClearColor(0.15, 0.1, 0.1, 1); //boja brisanja platna izmedu iscrtavanja dva okvira


	glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

	FPSManager FPSManagerObject(window, 60, 1.0, "3. lab. vjezba");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora
	//ova funkcija je isto tu u mainu.
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);



	/*********************************************************************************************/
	//indeksirani vrhovi i boje u odvojenim poljima. Konkretni podaci

	//svaki redak je jedna koordinata (x, y, z)
	/*float indeksiraniVrhovi[18] = {
			-1,  -1, 0,
			 1,  -1, 0,
			 0,   1, 0,
			-0.4, -0.1, 0,
			 0.4, -0.1, 0,
			 0,  -0.9, 0
	};

	//svaki redak je jedna boja vezana uz vrh na istoj poziciji kao u prethodnom polju (r, g, b)
	float indeksiraneBoje[18] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		0, 1, 1,
		1, 0, 1,
		1, 1, 0
	};

	//svaki redak je jedan trokut
	unsigned int indeksi[12] = {
		0, 5, 3,
		3, 5, 4,
		5, 1, 4,
		3, 4, 2
	}; */

	//ucitavanje sjencara i dohvat uniform varijable

	//GLint lokacijaUniformVarijable = glGetUniformLocation(sjencar->ID, "tMatrica"); //ovo pogledaj sta znaci

	/*********************************************************************************************/
	//prenosenje podataka i objasnjavanje u kojem formatu su ti podaci
	//generiranje buffera


	glGenVertexArrays(3, VAO);
	glGenBuffers(4, VBO);
	glGenBuffers(1, &EBO);

	//za kvadratic
	sjencar[0] = loadShader(argv[0], "shaderQuad");
	GLint uniformQuadColor = glGetUniformLocation(sjencar[0]->ID, "u_color");
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, model->colorVertices.size() * sizeof(model->colorVertices[0]), &model->colorVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(0);
	//glBindVertexArray(0); //otkvaèi

	/*std::vector<glm::vec3> tempVertices;
	std::vector<GLint> tempIndexes;
	tempVertices.push_back(glm::vec3(0.5, 0.01, 0.01));
	tempVertices.push_back(glm::vec3(0.01, 0.5, 0.01));
	tempVertices.push_back(glm::vec3(-0.5, 0.01, 0.01));
	tempVertices.push_back(glm::vec3(-1, 0.01, 0.01));

	tempIndexes.push_back(0);
	tempIndexes.push_back(1);
	tempIndexes.push_back(2);
	tempIndexes.push_back(1);
	tempIndexes.push_back(2);
	tempIndexes.push_back(3);*/

	sjencar[1] = loadShader(argv[0], "shader");
	//za trokute
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, tempVertices.size() * sizeof(glm::vec3), tempVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), model->vertices.data(), GL_STATIC_DRAW); //tu ide velièina buffera
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	//glBufferData(GL_ARRAY_BUFFER, tempVertices.size() * sizeof(glm::vec3), tempVertices.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, model->triangleColors.size() * sizeof(glm::vec3), model->triangleColors.data(), GL_STATIC_DRAW); //tu ide velièina buffera

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);


	//EBO za trokute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempIndexes.size() * sizeof(GLint), tempIndexes.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices.size() * sizeof(GLint), model->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); //otkvaèi, dobra programerska praksa

	//linije
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, model->bridges.size() * sizeof(glm::vec3), model->bridges.data(), GL_STATIC_DRAW); //tu ide velièina buffera
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0); //veliki upitnik
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	/*glBindVertexArray(VAO[2]);
	//buffer za koordinate i povezi s nultim mjestom u sjencaru -- layout (location = 0)
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraniVrhovi), indeksiraniVrhovi, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//buffer za boje i povezi s prvim mjestom u sjencaru -- layout (location = 1)
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraneBoje), indeksiraneBoje, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//buffer za indekse, moze biti samo jedan GL_ELEMENT_ARRAY_BUFFER po VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeksi), (void*)(&indeksi[0]), GL_STATIC_DRAW);
	glBindVertexArray(0); //otkvaèi, dobra programerska praksa

	/*********************************************************************************************/
	//"instanciranje objekata" svaka matrica reprezentira novu instancu objekta. Izrada polja transformacija koji postavljaju objekte 
	//u mrezu 4x4

	/*glm::mat4 jedinicna = glm::mat4(1);
	glm::mat4 skaliranje = glm::scale(jedinicna, glm::vec3(0.25, 0.25, 0.25));

	glm::mat4 poljeTransformacija[32];

	glm::vec3 osRotacije = glm::vec3(1, 0, 0);
	float kutRotacije = 3.1415 / 32;

	int brojac = 0;
	for (float i = -1; i < 1; i += 0.5) {
		for (float j = -1; j < 1; j += 0.5) {
			poljeTransformacija[brojac] = glm::translate(jedinicna, glm::vec3(j + 0.25, i + 0.25, 0))
				* skaliranje;

			poljeTransformacija[brojac + 16] = glm::translate(jedinicna, glm::vec3(j + 0.25, i + 0.25, 0)) *
				glm::rotate(jedinicna, brojac * kutRotacije, osRotacije) *
				glm::rotate(jedinicna, (float)3.141592, glm::vec3(0, 0, 1)) *
				skaliranje;
	brojac++;
}
	} */

	glm::vec3 p1(-0.5, 0, 0);
	glm::vec3 p2(0.5, 0, 0);
	//model->bridges.push_back(p1);
	//model->bridges.push_back(p2);


	/*********************************************************************************************/
	//glavna petlja za prikaz
	while (glfwWindowShouldClose(window) == false) {

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);

		/****************************/
		//osvjezavanje podataka, ovo je samo za rotaciju onog jednog trokuta 
		//poljeTransformacija[28] = poljeTransformacija[28] * glm::rotate(jedinicna, deltaTime * (float)3.141592 / 4, glm::vec3(0, 0, 1));

		//pobrisi platno
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/****************************/
		//iscrtavanje
		//za svaku instancu objekta saljemo naredbu za iscrtavanje. podaci o modelu ostaju na grafickoj, mijenja se samo uniform 
		//varijabla.
		glUseProgram(sjencar[0]->ID); //na 0 je za iscrtavat kvadart

		//quads
		glBindVertexArray(VAO[0]); //prvi vao
		//glUniformMatrix4fv(lokacijaUniformVarijable, 1, GL_FALSE, &poljeTransformacija[i][0][0]);
		//glDrawElements(GL_TRIANGLES, sizeof(indeksi), GL_UNSIGNED_INT, 0);
		glUniform3f(uniformQuadColor, model->currentColor.x, model->currentColor.y, model->currentColor.z);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0); //otkvaèi


		//glUseProgram(sjencar[1]->ID); //na 0 je za iscrtavat kvadart
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, model->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); //otkvaèi

		if (model->bridges.size() > 0) {
			model->updateable = false;
			model->bridges.insert(model->bridges.begin() + 1, glm::vec3(model->mapToGL(model->cursorPosition), 0));
			glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, model->bridges.size() * sizeof(glm::vec3), model->bridges.data(), GL_STATIC_DRAW); //tu ide velièina buffera
			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_LINE_STRIP, 0, model->bridges.size());
			glBindVertexArray(0); //otkvaèi
			model->bridges.erase(model->bridges.begin() + 1);
			model->updateable = true;
		}
		/*glBidVertexArray(VAO[2]);
		glDrawArrays(GL_LINE_STRIP, 0, model->bridges.size());
		glBindVertexArray(0); //otkvaèi
		if (model->bridges.size() > 2) {
			model->bridges.erase(model->bridges.begin() + 1);
		} */

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

	}

	delete sjencar[0]; //be careful to delete later
	delete sjencar[1];
	glDeleteBuffers(4, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(3, VAO);

	glfwTerminate();

	delete model;

	return EXIT_SUCCESS;
}
