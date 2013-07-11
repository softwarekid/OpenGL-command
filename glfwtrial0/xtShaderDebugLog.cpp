#include "StdAfx.h"
#include "xtShaderDebugLog.h"

#include <fstream>

#include <assert.h>


xtShaderDebugLog::xtShaderDebugLog(void)
{
}


xtShaderDebugLog::~xtShaderDebugLog(void)
{
}


void xtShaderlog::_print_programme_info_log () {
  int max_length = 2056;
  int actual_length = 0;
  char log[2056];
  glGetProgramInfoLog (m_programme_idx, max_length, &actual_length, log);
  printf ("program info log for GL index %i:\n%s", m_programme_idx, log);
}


const char* xtShaderlog::GL_type_to_string (const unsigned int& type) {
  if (GL_FLOAT == type) {
    return "GL_FLOAT";
  }
  if (GL_FLOAT_VEC2 == type) {
    return "GL_FLOAT_VEC2";
  }
  if (GL_FLOAT_VEC3 == type) {
    return "GL_FLOAT_VEC3";
  }
  if (GL_FLOAT_VEC4 == type) {
    return "GL_FLOAT_VEC4";
  }
  if (GL_FLOAT_MAT2 == type) {
    return "GL_FLOAT_MAT2";
  }
  if (GL_FLOAT_MAT3 == type) {
    return "GL_FLOAT_MAT3";
  }
  if ( GL_FLOAT_MAT4 == type) {
    return "GL_FLOAT_MAT4";
  }
  if (GL_INT == type) {
    return "GL_INT";
  }
  if (GL_BOOL == type) {
    return "GL_BOOL";
  }
  if (GL_SAMPLER_2D == type) {
    return "GL_SAMPLER_2D";
  }
  if (GL_SAMPLER_3D == type) {
    return "GL_SAMPLER_3D";
  }
  if (GL_SAMPLER_CUBE == type) {
    return "GL_SAMPLER_CUBE";
  }
  if (GL_SAMPLER_2D_SHADOW == type) {
    return "GL_SAMPLER_2D_SHADOW";
  }
  return "OTHER";
}



void xtShaderlog::print_all () {
  printf ("--------------------\nshader programme %i info:\n", m_programme_idx);
  int params = -1;
  glGetProgramiv (m_programme_idx, GL_LINK_STATUS, &params);
  char value[32];
  if (GL_TRUE == params) {
    strcpy (value, "GL_TRUE");
  } else {
    strcpy (value, "GL_FALSE");
  }
  printf ("GL_LINK_STATUS = %s\n", value);
  
  glGetProgramiv (m_programme_idx, GL_ATTACHED_SHADERS, &params);
  printf ("GL_ATTACHED_SHADERS = %i\n", params);
  if (m_has_vertex_shader) {
    printf ("  vertex shader index %i. file name: %s\n", m_vertex_shader_idx, m_vertex_shader_file_name.c_str ());
  }
  if (m_has_fragment_shader) {
    printf ("  fragment shader index %i. file name: %s\n", m_fragment_shader_idx, m_fragment_shader_file_name.c_str ());
  }
  
  glGetProgramiv (m_programme_idx, GL_ACTIVE_ATTRIBUTES, &params);
  printf ("GL_ACTIVE_ATTRIBUTES = %i\n", params);
  for ( int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    //char type_name[64];
    glGetActiveAttrib (m_programme_idx, i, max_length, &actual_length, &size, &type, name);
    if (size > 1) {
      for (int j = 0; j < size; j++) {
        char long_name[64];
        sprintf (long_name, "%s[%i]", name, j);
        int location = glGetAttribLocation (m_programme_idx, long_name);
        printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), long_name, location);
      }
    } else {
      int location = glGetAttribLocation (m_programme_idx, name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), name, location);
    }
  }
  
  glGetProgramiv (m_programme_idx, GL_ACTIVE_UNIFORMS, &params);
  printf ("GL_ACTIVE_UNIFORMS = %i\n", params);
  for ( int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    //char type_name[64];
    glGetActiveUniform (m_programme_idx, i, max_length, &actual_length, &size, &type, name);
    if (size > 1) {
      for (int j = 0; j < size; j++) {
        char long_name[64];
        sprintf (long_name, "%s[%i]", name, j);
        int location = glGetUniformLocation (m_programme_idx, long_name);
        printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), long_name, location);
      }
    } else {
      int location = glGetUniformLocation (m_programme_idx, name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), name, location);
    }
  }
  
  _print_programme_info_log ();
}

bool xtShaderlog::is_valid () {
  glValidateProgram (m_programme_idx);
  int params = -1;
  glGetProgramiv (m_programme_idx, GL_VALIDATE_STATUS, &params);
  //char value[32];
  if (GL_TRUE != params) {
    printf ("program %i GL_VALIDATE_STATUS = GL_FALSE\n", m_programme_idx);
    _print_programme_info_log ();
    return false;
  }
  printf ("program %i GL_VALIDATE_STATUS = GL_TRUE\n", m_programme_idx);
  return true;
}

void xtShaderlog::printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}


// code from the site:
// http://lazyfoo.net/tutorials/OpenGL/30_loading_text_file_shaders/index.php
 /*
        Pre Condition:
         -None
        Post Condition:
         -Returns the ID of a compiled shader of the specified type from the specified file
         -Reports error to console if file could not be found or compiled
        Side Effects:
         -None
        */
GLuint xtShaderlog::LoadShaderFromFile( const char *path, GLenum shaderType )
{
	GLuint shaderId = 0;

	std::string shaderString;
	std::ifstream sourceFile( path );

	if ( sourceFile ) {
		// get string shader source from file
		shaderString.assign( ( std::istreambuf_iterator< char > (sourceFile)), 
			std::istreambuf_iterator< char > ());
		// set shader ID
		shaderId = glCreateShader( shaderType );

		// Set shader source
		const GLchar *shaderSource = shaderString.c_str();
		glShaderSource( shaderId, 1, (const GLchar **)&shaderSource, NULL);

		// Compile shader source
		glCompileShader( shaderId );

		// Check shader for errors
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
		if ( shaderCompiled != GL_TRUE ) {
			printf("Unable to complie shader %d!\n\n Source:\n%s\n",shaderId, shaderSource);
			printShaderLog( shaderId );
			glDeleteShader( shaderId );
		}

	} else {
		printf("Undable to open file %s\n",path);
	}

	return shaderId;
}

void xtShaderlog::LoadCreateVFShader(const char *vsPath, const char *fsPath)
{
	m_has_vertex_shader = true;
	m_has_fragment_shader = true;

	m_vertex_shader_file_name = vsPath;
	m_fragment_shader_file_name = fsPath;

	m_vertex_shader_idx = LoadShaderFromFile(vsPath,GL_VERTEX_SHADER);
	m_fragment_shader_idx = LoadShaderFromFile(fsPath,GL_FRAGMENT_SHADER);

	assert(m_vertex_shader_idx);
	assert(m_fragment_shader_idx);
	// Befor the link need to chek if the vertex & fragement shader create succeed!

	m_programme_idx = glCreateProgram ();
	glAttachShader (m_programme_idx, m_vertex_shader_idx);
	glAttachShader (m_programme_idx, m_fragment_shader_idx);
	glLinkProgram (m_programme_idx/*shader_programme*/);

}

