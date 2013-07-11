// glfwtrial0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <string>
#include <fstream>

#include "xtGLTrial2.h"
#include "xtGLTrial3.h"

std::ofstream g_log_file;

bool Start_Log( const char *file_name) 
{
	g_log_file.open(file_name);
	if ( !g_log_file.is_open() ) {
		printf("ERROR: could not open log file % for writing\n",file_name);
		return false;
	}
	g_log_file << "GL log file "<< std::endl;
	return true;
}

void log_gl_state () {
  g_log_file << "-------------------------------------------------------------------\n";
  g_log_file << "GL State Machine Dump:\n";
  int v = 0;
  glGetIntegerv (GL_ACTIVE_TEXTURE, &v);
  g_log_file << "GL_ACTIVE_TEXTURE" << " " << v - GL_TEXTURE0 << "\n";
  unsigned char b = GL_FALSE;
  glGetBooleanv (GL_ALPHA_TEST, &b);
  if (GL_TRUE == b) {
    g_log_file << "GL_ALPHA_TEST" << " GL_TRUE\n";
  } else {
    g_log_file << "GL_ALPHA_TEST" << " GL_FALSE\n";
  }
  glGetBooleanv (GL_CULL_FACE, &b);
  if (GL_TRUE == b) {
    g_log_file << "GL_CULL_FACE" << " GL_TRUE\n";
  } else {
    g_log_file << "GL_CULL_FACE" << " GL_FALSE\n";
  }
  glGetIntegerv (GL_CULL_FACE_MODE, &v);
  if (GL_FRONT == v) {
    g_log_file << "GL_CULL_FACE_MODE" << " GL_FRONT\n";
  } else if (GL_BACK == v) {
    g_log_file << "GL_CULL_FACE_MODE" << " GL_BACK\n";
  } else {
    g_log_file << "GL_CULL_FACE_MODE" << " GL_FRONT_AND_BACK\n";
  }
  glGetIntegerv (GL_CURRENT_PROGRAM, &v);
  g_log_file << "GL_CURRENT_PROGRAM" << " " << v << "\n";
  float f = 0.0f;
  glGetFloatv (GL_DEPTH_CLEAR_VALUE, &f);
  g_log_file << "GL_DEPTH_CLEAR_VALUE" << " " << f << "\n";
  glGetBooleanv (GL_DEPTH_TEST, &b);
  if (GL_TRUE == b) {
    g_log_file << "GL_DEPTH_TEST" << " GL_TRUE\n";
  } else {
    g_log_file << "GL_DEPTH_TEST" << " GL_FALSE\n";
  }
  glGetBooleanv (GL_DEPTH_WRITEMASK, &b);
  if (GL_TRUE == b) {
    g_log_file << "GL_DEPTH_WRITEMASK" << " GL_TRUE\n";
  } else {
    g_log_file << "GL_DEPTH_WRITEMASK" << " GL_FALSE\n";
  }
  glGetIntegerv (GL_FRONT_FACE, &v);
  if (GL_CW == v) {
    g_log_file << "GL_FRONT_FACE" << " GL_CW\n";
  } else {
    g_log_file << "GL_FRONT_FACE" << " GL_CCW\n";
  }
  g_log_file << "-------------------------------------------------------------------"<<std::endl;
}



class xtGLTrial
{

};



int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();

	int major = 4;
	int minor = 2;
	//glfwOpenWindowHint 
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,major);

	//glfwOpenWindow(640,480,8,8,8,8,24,8,GLFW_WINDOW);
	Start_Log("glfwtrial0.log");
	g_log_file << "Create Windows\n";
    GLFWwindow *window = glfwCreateWindow(640, 480, "Demo", NULL, NULL);
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

	log_gl_state();

	//==================================================================
	//xtGLTrial2 gltrialpro;
	//gltrialpro.CreateVBO();
	//log_gl_state();
	//gltrialpro.Draw(window);
	xtGLTrial3 gltrial3;
	gltrial3.CreateVBO();
	gltrial3.Render(window);


	glfwTerminate();


	return 0;
}

