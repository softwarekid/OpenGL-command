#pragma once
#include "xtglutcontrol.h"

class txFemSurf;

class xtFemViewer :
	public xtGlutControl
{
public:
	xtFemViewer(const char *title, int width, int height);
	~xtFemViewer(void);

	void LoadFem(const char *filename);

  /// draw the scene
  virtual void draw(const std::string& _draw_mode);

private:
	txFemSurf *femsurf;
};

