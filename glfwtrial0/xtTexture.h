#pragma once
#include <string>


enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class xtTexture
{
public:
	xtTexture(void);
	~xtTexture(void);

	bool LoadTexture2D( std::string a_sPath, bool bGenerateMipMaps = false );
	void BindTexture(int iTextureUnit = 0);

	void SetFiltering(int a_tfMagnification, int a_tfMinification);

	int GetMinficationFilter();
	int GetMagnificationFilter();

	void ReleaseTexture();

private:
	int iWidth, iHeight, iBPP; // Byte per pixel
	unsigned int uiTexture;
	unsigned int uiSampler;
	bool bMipMapGenerated;

	int tfMinification, tfMagnification;

	std::string sPath;
};

