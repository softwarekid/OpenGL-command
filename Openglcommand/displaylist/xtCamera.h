#pragma once
#include "Matrices.h"

class xtCamera
{
	friend class xtViewer;
public:
	xtCamera(void);
	~xtCamera(void);

	void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

	void setCameraX(float x)        { cameraPosition[0] = x; updateViewMatrix(); }
    void setCameraY(float y)        { cameraPosition[1] = y; updateViewMatrix(); }
    void setCameraZ(float z)        { cameraPosition[2] = z; updateViewMatrix(); }
    void setCameraAngleX(float p)   { cameraAngle[0] = p; updateViewMatrix(); }
    void setCameraAngleY(float h)   { cameraAngle[1] = h; updateViewMatrix(); }
    void setCameraAngleZ(float r)   { cameraAngle[2] = r; updateViewMatrix(); }
    float getCameraX()              { return cameraPosition[0]; }
    float getCameraY()              { return cameraPosition[1]; }
    float getCameraZ()              { return cameraPosition[2]; }
    float getCameraAngleX()         { return cameraAngle[0]; }
    float getCameraAngleY()         { return cameraAngle[1]; }
    float getCameraAngleZ()         { return cameraAngle[2]; }

private:
	void updateViewMatrix();

private:

	float cameraPosition[3];
	float cameraAngle[3];

	Matrix4 matrixView;
};

