#pragma once
#include "glcommon.h"

class xtShaderlog;
class xtGLTexture
{
public:
	xtGLTexture(void);
	~xtGLTexture(void);

	void LoadTexture(char *filename);
	void CreateVBO();
	void Render(GLFWwindow *window);
private:
	GLuint mTex2d;
	GLuint vao;
	GLuint vbo;
	unsigned int shader_programme;

	xtShaderlog *shaderlog;
	unsigned int mGLMatrixIdx;
};

