#include "StdAfx.h"
#include "xtGLTrial3.h"
#include <assert.h>

#include "xtShaderDebugLog.h"



xtGLTrial3::xtGLTrial3(void)
{
	shaderlog = new xtShaderlog;
}


xtGLTrial3::~xtGLTrial3(void)
{
	delete shaderlog;
}

void xtGLTrial3::Render(GLFWwindow *window)
{
bool running = true;
while (running) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transformed
  glUseProgram (shader_programme);
  glBindVertexArray (vao);
  glDrawArrays (GL_TRIANGLES, 0, 3);
  //glfwSwapBuffers(window);


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
  glDrawArrays (GL_TRIANGLES, 0, 3);


  glfwSwapBuffers(window);

  running = !glfwGetKey(window,GLFW_KEY_ESCAPE) ;

  //shaderlog->print_all();
}
}


void xtGLTrial3::CreateVBO()
{
float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

float colours[] = {
  1.0f, 0.0f,  0.0f,
  0.0f, 1.0f,  0.0f,
  0.0f, 0.0f,  1.0f
};

unsigned int points_vbo = 0;
glGenBuffers (1, &points_vbo);
glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), &points, GL_STATIC_DRAW);

unsigned int colours_vbo = 0;
glGenBuffers (1, &colours_vbo);
glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), &colours, GL_STATIC_DRAW);


//==============================================================================
assert(points_vbo);
assert(colours_vbo);

 vao = 0;
glGenVertexArrays (1, &vao);
glBindVertexArray (vao);

glEnableVertexAttribArray (0);
glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
glEnableVertexAttribArray (1);
glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

assert(vao);
//==============================================================================




const char* vsPath = "trial3vertex_vs.glsl";
const char* fsPath = "trial3frame_fs.glsl";
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

}

/****
comment from the site
http://www.antongerdelan.net/opengl/vertexbuffers.html

About the enable
Enable Vertex Arrays 0 and 1 in the State Machine

Okay, so we created a vertex array object, and described 2 attribute "pointers" 
within it. Unfortunately, attributes are disabled by default in OpenGL 4. 
We need to explicitly enable them too. This is easy to get wrong or overlook, 
and is not well explained in the documentation. We use a function called 
glEnableVertexAttribArray() to enable each one. This function only affects 
the currently bound vertex array object. This means that when we do this now, 
it will only affect our attributes, above. We will need to bind every new 
vertex array and repeat this procedure for those too.


We are using 2 attributes (points and colours), and we know that these are 
numbered 0 (points), and 1 (colours); matching the numbers we gave when setting up 
the vertex attribute pointers, earlier.

A little aside about this...
Other GL incarnations don't have vertex attribute objects, and need to explicitly 
enable and disable the attributes every time a new type of object is drawn, 
which changes the enabled attributes globally in the state machine. We don't 
need to worry about that in OpenGL 4; the vertex attribute object will remember 
its enabled attributes. i.e. they are no longer global states. This isn't mentioned 
anywhere in the official documentation, nor is it explained properly 
in other tutorials - it's very easy to get a false-positive misunderstanding of 
how these things work and run into hair-pulling problems later when you're trying to 
draw 2 different shapes. I actually figured this out by writing a programme 
that used the scientific method to try and falsify (break in every conceivable way) 
my theory for how the logic of attribute enabling worked; if I'd just tried it until 
it worked (deduction) then I'd have been wrong! Well...I just don't know what to 
say...some sort of prize is in order for the designers of this.


****/


/****
"Winding" and Back-Face Culling

The last thing that you should know about is a built-in rendering optimisation 
called back-face culling. This gives a hint to GL so that it can throw away 
the hidden "back" or inside faces of a mesh. This should remove half of 
the vertex shaders, and half of the fragment shader instances from the GPU 
- allowing you to render things twice as large in the same time. It's not appropriate
all of the time - you might want our 2d triangle to spin and show both sides.

The only thing that you need to do is specify if your the clock-wise vertex "winding"
order produces the front, or the back of each face, and to enable culling of that side.
Our triangle, you can see, is given in clock-wise order. Most mesh formats actually 
go the other way, so it pays to test this before wondering why a mesh isn't showing up
at all!


Try switching to counter clock-wise to make sure that the triangle disappears. If you
were to rotate it around now you'd see the other side was visible. As with other GL 
states, this culling is enabled globally, in the state machine, you can enable and 
disable it between calls to glDrawArrays so that some objects are double-sided, 
and some are single-sided, etc. Keep in mind which winding order you are making 
new shapes in..
****/
