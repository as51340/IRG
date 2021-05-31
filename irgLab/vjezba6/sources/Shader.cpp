#include "Shader.h"
#include <iostream>
void Shader::checkCompilerErrors(unsigned int shader, std::string type)
{
	int success;
	char infolog[1024] = {};
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infolog);
			fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %d\n%s\n-----------------------------------------------------\n", type, infolog);
			//std::cout << "Ne radi dobro" << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infolog);
			fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %d\n%s\n-------------------------------------------------------\n", type, infolog);
			//std::cout << "Ne radi dobro" << std::endl;
		}
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geomPath)
{
	//std::cout << vertexPath << std::endl;
	std::string vertexCode;
	std::string fragmentCode;
	std::string geomCode; //new

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile; //new

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); //new
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geomPath); //new

		std::stringstream vShaderStream, fShaderStream, gShaderStream; //modified
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf(); //new

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close(); //new
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geomCode = gShaderStream.str(); //new
	}
	catch (std::ifstream::failure e)
	{
		fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}


	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geomCode.c_str(); //new

	// 2. compile shaders
	unsigned int vertex, fragment, geom; //modified
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompilerErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompilerErrors(fragment, "FRAGMENT");

	//geom shader
	geom = glCreateShader(GL_GEOMETRY_SHADER); //add
	glShaderSource(geom, 1, &gShaderCode, NULL);
	glCompileShader(geom);
	checkCompilerErrors(geom, "GEOM");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, geom); //add
	glAttachShader(ID, fragment);

	glLinkProgram(ID);
	checkCompilerErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geom); //add
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{

	glUseProgram(ID);
}

void Shader::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);

}

void Shader::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);

}
