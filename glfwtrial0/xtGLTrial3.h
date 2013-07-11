#pragma once
#include "glcommon.h"

class xtShaderlog;
class xtGLTrial3
{
public:
	xtGLTrial3(void);
	~xtGLTrial3(void);

	void CreateVBO();

	void Render(GLFWwindow *window);

private:
	unsigned int vbo;
	unsigned int vao;
	unsigned int shader_programme;
	xtShaderlog *shaderlog;
};

