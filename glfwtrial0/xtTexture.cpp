#include "StdAfx.h"
#include "xtTexture.h"
#include "../soil/Simple OpenGL Image Library/src/SOIL.h"
#include "glcommon.h"

xtTexture::xtTexture(void)
{
	this->bMipMapGenerated = false;
}


xtTexture::~xtTexture(void)
{
}

bool xtTexture::LoadTexture2D( std::string a_sPath, bool bGenerateMipMaps /*= false*/ )
{
	uiTexture = SOIL_load_OGL_texture(a_sPath.c_str(), 
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	this->iWidth = -1;
	this->iHeight = -1;
	this->iBPP = -1;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	

	// got the size of the picture an generate the sample min
	sPath = a_sPath;
	return true;
}

void xtTexture::BindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	// glBindSampler(iTextureUnit, uiSampler);
}

void xtTexture::ReleaseTexture()
{
	// release smapler first
	glDeleteTextures(1, &uiTexture);
}

