// glfwtrial0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <string>

class xtGLTrial
{
public:
	void CreateVBO();

	void Draw(GLFWwindow *window);

	unsigned int vbo;
	unsigned int vao;
	unsigned int shader_programme;
};

void xtGLTrial::Draw(GLFWwindow *window)
{
	bool running = true;
while (running) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (shader_programme);
  glBindVertexArray (vao);
  glDrawArrays (GL_TRIANGLES, 0, 3);
  glfwSwapBuffers(window);
  running = !glfwGetKey(window, GLFW_KEY_ESCAPE) ;
}
}

void xtGLTrial::CreateVBO()
{
	float points[] = {
     0.0f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };

	   vbo= 0;
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), &points[0], GL_STATIC_DRAW);


     vao = 0;
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  glEnableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


  std::string vertex_shader = "";
vertex_shader+="in vec3 vp;";
vertex_shader+="void main () {" ;
vertex_shader+="  gl_Position = vec4 (vp, 1.0);" ;
vertex_shader+="}";

std::string fragment_shader = "";
fragment_shader+="out vec4 frag_colour;";
fragment_shader+="void main () {";
fragment_shader+="  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);";
fragment_shader+="}";

unsigned int vs = glCreateShader (GL_VERTEX_SHADER);
const char* str = vertex_shader.c_str ();
glShaderSource (vs, 1, &str, NULL);
glCompileShader (vs);
unsigned int fs = glCreateShader (GL_FRAGMENT_SHADER);
const char* strb = fragment_shader.c_str ();
glShaderSource (fs, 1, &strb, NULL);
glCompileShader (fs);

 shader_programme = glCreateProgram ();
glAttachShader (shader_programme, fs);
glAttachShader (shader_programme, vs);
glLinkProgram (shader_programme);
}


int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();

	int major = 4;
	int minor = 2;
	//glfwOpenWindowHint 
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,major);

	//glfwOpenWindow(640,480,8,8,8,8,24,8,GLFW_WINDOW);

    GLFWwindow *window = glfwCreateWindow(500, 500, "Split view demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window\n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Enable vsync
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

	GLenum err = glewInit();

	if ( GLEW_OK != err ) {
		fprintf(stderr,"Error: %s\n", glewGetErrorString(err));
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n",renderer);
	printf("OpenGL Version supported: %s\n",version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//==================================================================
	xtGLTrial gltrialpro;
	gltrialpro.CreateVBO();
	gltrialpro.Draw(window);

	glfwTerminate();


	return 0;
}

