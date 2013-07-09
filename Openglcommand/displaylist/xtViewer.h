#pragma once
#include "Matrices.h"
#include <vector>

class xtCamera;
class xtModelP;
class xtViewer
{
public:
	xtViewer(void);
	~xtViewer(void);

	void setCamera(xtCamera *camera);
    void setDrawMode(int mode);
    void setWindowSize(int width, int height);

	void AddModel(xtModelP *model);

	void Draw();

private:
	void setViewport(int x, int y, int width, int height);

	void setFrustum(float l, float r, float b, float t, float n, float f);
    void setFrustum(float fovy, float ratio, float n, float f);
    void setOrthoFrustum(float l, float r, float b, float t, float n=-1, float f=1);

    void drawGrid(float size, float step);          // draw a grid on XZ plane
	void drawGridXY(float size, float step);
	void drawGridYZ(float size, float step);
    void drawAxis(float size);                      // draw 3 axis

private:
	Matrix4 matrixProjection;
	// as a cache;
	Matrix4 matrixModelView;
	int windowWidth;
	int windowHeight;

	int drawMode;

	xtCamera *camera;

	std::vector<xtModelP *> modellist;
};

