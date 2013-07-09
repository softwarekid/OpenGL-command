#pragma once
#include "Matrices.h"

class xtModelP
{
public:
	xtModelP(void);
	virtual ~xtModelP(void);

	virtual void Draw()=0;

	Matrix4 &GetModelMatrix() { return this->matrixModel; };

	void SetModelPosition(float *position) 
	{
		for ( int i=0; i<3; ++i ) {
			this->modelPosition[i] = position[i];
		}
	}

	void SetModelAngle(float *angle) 
	{
		for ( int i=0; i<3; ++i )
			this->modelAngle[i] = angle[i];
	}

private:
	float modelPosition[3];
    float modelAngle[3];

	Matrix4 matrixModel;
};

