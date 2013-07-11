#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class xtShaderlog;
class xtGLTrial2
{
public:
	xtGLTrial2(void);
	~xtGLTrial2(void);

	public:
	void CreateVBO();

	void Draw(GLFWwindow *window);

private:
	void Initialize();
	void DestroyMem();

private:
	unsigned int vbo;
	unsigned int vao;
	unsigned int shader_programme;
	xtShaderlog *shaderlog;
};

