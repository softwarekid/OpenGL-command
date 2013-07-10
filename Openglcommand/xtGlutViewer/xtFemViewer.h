#pragma once
#include "xtglutcontrol.h"

class txFemSurf;

class xtFemViewer :
	public xtGlutControl
{
public:
	xtFemViewer(const char *title, int width, int height);
	~xtFemViewer(void);

	virtual void mouse(int button, int state, int x, int y);

	void LoadFem(const char *filename);

  /// draw the scene
  virtual void draw(const std::string& _draw_mode);

private:
	txFemSurf *femsurf;


	bool isHit;
	int hitElementType;
	int hitElementIndex;
};

