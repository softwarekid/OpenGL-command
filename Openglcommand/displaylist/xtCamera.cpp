#include "StdAfx.h"
#include "xtCamera.h"

const float CAMERA_ANGLE_X = 45.0f;     // pitch in degree
const float CAMERA_ANGLE_Y = -45.0f;    // heading in degree
const float CAMERA_DISTANCE = 25.0f;    // camera distance


xtCamera::xtCamera(void)
{
	//this->matrixView.identity();
	for ( int i=0; i<3; ++i ) {
		this->cameraPosition[i] = 0.0;
	}

	this->cameraAngle[0] = CAMERA_ANGLE_X;
	this->cameraAngle[1] = CAMERA_ANGLE_Y;
	this->cameraAngle[2] = 0.0f;

	updateViewMatrix();
}


xtCamera::~xtCamera(void)
{
}

void xtCamera::updateViewMatrix()
{
	matrixView.identity();
	matrixView.translate(-cameraPosition[0],-cameraPosition[1],-cameraPosition[2]);
	matrixView.rotateX(-cameraAngle[0]);
	matrixView.rotateY(-cameraAngle[1]);
	matrixView.rotateZ(-cameraAngle[2]);


}

void xtCamera::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	float forward[4];
    float up[4];
    float left[4];
    float position[4];
    float invLength;

    // determine forward vector (direction reversed because it is camera)
    forward[0] = posX - targetX;    // x
    forward[1] = posY - targetY;    // y
    forward[2] = posZ - targetZ;    // z
    forward[3] = 0.0f;              // w
    // normalize it without w-component
    invLength = 1.0f / sqrtf(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    forward[0] *= invLength;
    forward[1] *= invLength;
    forward[2] *= invLength;

    // assume up direction is straight up
    up[0] = 0.0f;   // x
    up[1] = 1.0f;   // y
    up[2] = 0.0f;   // z
    up[3] = 0.0f;   // w

    // compute left vector with cross product
    left[0] = up[1]*forward[2] - up[2]*forward[1];  // x
    left[1] = up[2]*forward[0] - up[0]*forward[2];  // y
    left[2] = up[0]*forward[1] - up[1]*forward[0];  // z
    left[3] = 1.0f;                                 // w

    // re-compute orthogonal up vector
    up[0] = forward[1]*left[2] - forward[2]*left[1];    // x
    up[1] = forward[2]*left[0] - forward[0]*left[2];    // y
    up[2] = forward[0]*left[1] - forward[1]*left[0];    // z
    up[3] = 0.0f;                                       // w

    // camera position
    position[0] = -posX;
    position[1] = -posY;
    position[2] = -posZ;
    position[3] = 1.0f;

	for ( int i=0; i<3; ++i ) {
		this->cameraPosition[i] = -position[i];
	}

    // copy axis vectors to matrix
    matrixView.identity();
    matrixView.setColumn(0, left);
    matrixView.setColumn(1, up);
    matrixView.setColumn(2, forward);
    matrixView.setColumn(3, position);
}
