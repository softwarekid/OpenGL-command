#pragma once
#include <string>
class xtShader
{
public:
	xtShader(void);
	~xtShader(void);

	bool LoadShader(std::string sFile, int a_iType);
	void DeleteShader();

	bool IsLoaded();
	unsigned int GetShaderID();

private:
	unsigned int uiShader; // ID of shader
	int iType; //GL_VERTEX_SHADER, GL_FRAGMENT_SHADER;
	bool bLoaded;
};

class xtShaderProgram
{
public:
	xtShaderProgram();

	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(xtShader *shShader);
	bool LinkProgram();

	void UseProgram();

	unsigned int GetProgramID();

private:
	unsigned int uiProgram;
	bool bLinked;

};