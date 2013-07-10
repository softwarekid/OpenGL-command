#pragma once
#include "Vectors.h"

class xtRayTriOverlay
{
public:
	xtRayTriOverlay(void);
	~xtRayTriOverlay(void);
};

 bool IntersectTriangle(const Vector3& orig, const Vector3& dir,
     Vector3& v0, Vector3& v1, Vector3& v2,
     float* t, float* u, float* v);

