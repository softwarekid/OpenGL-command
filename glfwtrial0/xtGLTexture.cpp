#include "StdAfx.h"
#include "xtGLTexture.h"
#include <assert.h>
#include "../soil/Simple OpenGL Image Library/src/soil.h"
#include "xtShaderDebugLog.h"


xtGLTexture::xtGLTexture(void)
{
	shaderlog = new xtShaderlog;
}


xtGLTexture::~xtGLTexture(void)
{
	delete shaderlog;
}

void xtGLTexture::LoadTexture(char *filename)
{
	mTex2d = SOIL_load_OGL_texture(filename, 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if ( 0==mTex2d ) {
		printf("SOID loading error: '%s'\n", SOIL_last_result());
	}
}

void xtGLTexture::CreateVBO()
{
float points[] = {
   -0.5f,  -0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
   0.5f, 0.5f,  0.0f,
   -0.5f,  -0.5f,  0.0f,
    0.5f, 0.5f,  0.0f,
   -0.5f, 0.5f,  0.0f
};

float colours[] = {
  1.0f, 0.0f,  0.0f,
  0.0f, 1.0f,  0.0f,
  0.0f, 0.0f,  1.0f
};

float texcoords[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	0.0f,1.0f,
};

unsigned int vt_vbo;
glGenBuffers(1,&vt_vbo);
glBindBuffer(GL_ARRAY_BUFFER,vt_vbo);
int dimensions = 2;
int length = 6;
glBufferData( GL_ARRAY_BUFFER, dimensions*length*sizeof(float),texcoords,GL_STATIC_DRAW);

unsigned int pointsvbo = 0;
glGenBuffers(1, &pointsvbo);
glBindBuffer(GL_ARRAY_BUFFER,pointsvbo);
glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), &points, GL_STATIC_DRAW);

unsigned int colorsvbo = 0;
glGenBuffers(1, &colorsvbo);
glBindBuffer(GL_ARRAY_BUFFER, colorsvbo);

assert(pointsvbo);
assert(colorsvbo);

vao = 0;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
//glEnableVertexAttribArray(1);
//glBindBuffer(GL_ARRAY_BUFFER, colorsvbo);
//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(GLubyte*)NULL);
glEnableVertexAttribArray (1); // don't forget this!
//glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
dimensions = 2; // 2d data for texture coords
glVertexAttribPointer (1, dimensions, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


assert(vao);

const char* vsPath = "xtgltexture_vs.glsl";
const char* fsPath = "xtgltexture_fs.glsl";
shaderlog->LoadCreateVFShader(vsPath,fsPath);
shaderlog->print_all();

this->shader_programme = shaderlog->GetProgrammeIdx();

// pay special attension to this column 
float matrix[] = {
  1.0f, 0.0f, 0.0f, 0.0f, // first column
  0.0f, 1.0f, 0.0f, 0.0f, // second column
  0.0f, 0.0f, 1.0f, 0.0f, // third column
  0.5f, 0.0f, 0.0f, 1.0f // fourth column
};

mGLMatrixIdx = glGetUniformLocation (shader_programme, "xtmatrix");
glUseProgram (shader_programme);
glUniformMatrix4fv (mGLMatrixIdx, 1, GL_FALSE, matrix);

// bind texture
glActiveTexture (GL_TEXTURE0); // activate the first slot
glBindTexture (GL_TEXTURE_2D, mTex2d);



}

void xtGLTexture::Render(GLFWwindow *window)
{
	bool running = true;
	while ( running ) {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( this->shader_programme );
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// pay special attension to this column 
		float matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.0f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.0f, 0.0f, // third column
		0.0f, 0.0f, 0.0f, 1.0f // fourth column
		};

		  // identical
	  glUseProgram (shader_programme);
	  glUniformMatrix4fv (mGLMatrixIdx, 1, GL_FALSE, matrix);
	  glBindVertexArray (vao);
	  glDrawArrays (GL_TRIANGLES, 0, 6);


	  glfwSwapBuffers(window);

	  running = !glfwGetKey(window,GLFW_KEY_ESCAPE) ;


	}
}
