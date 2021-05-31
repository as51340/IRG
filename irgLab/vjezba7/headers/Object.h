#pragma once

#include "Tranform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Curve.h"

class Object {

public:
	Mesh mesh;
	Shader* sjencar;
	Transform transform;
	char* shaderName;

	Object(Mesh&, char*);

	void loadShader(char* path, char* naziv);

	void transformObject();
};
