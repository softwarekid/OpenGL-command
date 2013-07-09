#pragma once
#include "xtVec3d.h"

class xtUtil
{
public:
	xtUtil(void);
	~xtUtil(void);
};

xtVec3df Min(const xtVec3df &l, const xtVec3df &r) {
	xtVec3df temp;
	for ( int i=0; i<3; ++i ) {
		temp.v[i] = l.v[i] < r.v[i] ? l.v[i] : r.v[i];
	}

	return temp;
}

xtVec3df Max(const xtVec3df &l, const xtVec3df &r) {
	xtVec3df temp;
	for ( int i=0; i<3; ++i ) {
		temp.v[i] = l.v[i] > r.v[i] ? l.v[i] :r.v[i];
	}

	return temp;
}

