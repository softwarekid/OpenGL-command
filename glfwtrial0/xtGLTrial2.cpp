#include "StdAfx.h"
#include "xtGLTrial2.h"

#include <string>

#include "xtShaderDebugLog.h"


xtGLTrial2::xtGLTrial2(void)
{
	Initialize();
}

void xtGLTrial2::Initialize()
{
	shaderlog = new xtShaderlog;
}


xtGLTrial2::~xtGLTrial2(void)
{
	DestroyMem();
}

void xtGLTrial2::DestroyMem()
{
	if ( shaderlog ) {
		delete shaderlog;
		shaderlog = NULL;
	}
}

void xtGLTrial2::Draw(GLFWwindow *window)
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

void xtGLTrial2::CreateVBO()
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

//shaderlog
shaderlog->Set(shader_programme,vs,fs,"vertess", "framess");
shaderlog->print_all();
}

