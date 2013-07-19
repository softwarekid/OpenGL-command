#include "StdAfx.h"
#include "xtShader.h"
#include <vector>
#include "glcommon.h"

xtShader::xtShader(void)
{
	this->bLoaded = false;
}


xtShader::~xtShader(void)
{
}

/*-----------------------------------------------
Name:	loadShader
Params:	sFile - path to a file
		a_iType - type of shader (fragment, vertex, geometry)
Result:	Loads and compiles shader.
/*---------------------------------------------*/
bool xtShader::LoadShader(std::string sFile, int a_iType)
{
	FILE *fp = fopen(sFile.c_str(), "rt");
	if (!fp) return false;

	std::vector<std::string> sLines;
	char sLine[255];
	while(fgets(sLine,255,fp)) {
		sLines.push_back(sLine);
	}
	fclose(fp);

	const char** sProgram = new const char*[sLines.size()];
	for ( size_t i=0; i<sLines.size(); ++i ) {
		sProgram[i] = sLines[i].c_str();
	}

	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader,sLines.size(),sProgram,NULL);
	glCompileShader(uiShader);

	delete[] sProgram;
	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if ( iCompilationStatus == GL_FALSE) return false;

	iType = a_iType;
	bLoaded = true;

	return true;
}

/*-----------------------------------------------
Name:	isLoaded
Params:	none
Result:	True if shader was loaded and compiled.
/*---------------------------------------------*/
bool xtShader::IsLoaded()
{
	return this->bLoaded;
}

/*-----------------------------------------------
Name:	getShaderID
Params:	none
Result:	Returns ID of a generated shader.
/*---------------------------------------------*/
unsigned int xtShader::GetShaderID()
{
	return uiShader;
}


/*-----------------------------------------------
Name:	deleteShader
Params:	none
Result:	Deletes shader and frees memory in GPU.
/*---------------------------------------------*/
void xtShader::DeleteShader()
{
	if (!IsLoaded()) return;
	bLoaded = false;
}

xtShaderProgram::xtShaderProgram()
{
	bLinked = false;
}


void xtShaderProgram::CreateProgram()
{
	uiProgram = glCreateProgram();
}


bool xtShaderProgram::AddShaderToProgram(xtShader *shShader)
{
	if ( !shShader->IsLoaded()) return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}

bool xtShaderProgram::LinkProgram()
{
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

void xtShaderProgram::DeleteProgram()
{
	if (!bLinked) return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

void xtShaderProgram::UseProgram()
{
	if (bLinked) glUseProgram(uiProgram);
}

unsigned int xtShaderProgram::GetProgramID()
{ 
	return uiProgram;
}



