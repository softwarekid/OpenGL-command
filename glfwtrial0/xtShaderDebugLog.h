#pragma once
#include <GL/glew.h>
#include <string>

class xtShaderDebugLog
{
public:
	xtShaderDebugLog(void);
	~xtShaderDebugLog(void);
};

class xtShaderlog
{
public:

	void Set(unsigned int programmeIndex, unsigned int vertexShaderIndex, unsigned int frameShaderIndex, 
			const char *vertexShaderFileName, const char *fragmentShaderFileName,
		bool hasVertexShader=true, bool hasFragmentShader=true) 
	{
		m_has_vertex_shader = (hasVertexShader);
		m_has_fragment_shader = (hasFragmentShader);
		m_programme_idx = (programmeIndex);
		m_vertex_shader_idx = vertexShaderIndex;
		m_fragment_shader_idx = frameShaderIndex;
		m_vertex_shader_file_name = vertexShaderFileName;
		m_fragment_shader_file_name = fragmentShaderFileName;
	}

	void LoadCreateVFShader(const char *vsPath, const char *fsPath);

	unsigned int GetProgrammeIdx() { return m_programme_idx; };

	void print_all();
	bool is_valid();

public:
	static GLuint LoadShaderFromFile( const char *path, GLenum shaderType );
	static void printShaderLog( GLuint shader );

private:
	void _print_programme_info_log();
	const char* GL_type_to_string (const unsigned int& type);

private:
	unsigned int m_programme_idx;
	bool m_has_vertex_shader;
	bool m_has_fragment_shader;

	std::string m_vertex_shader_file_name;
	std::string m_fragment_shader_file_name;

	unsigned int m_vertex_shader_idx;
	unsigned int m_fragment_shader_idx;
};




