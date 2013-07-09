#pragma once
#include "xtmodelp.h"

class xtDrawTeapot :
	public xtModelP
{
public:
	xtDrawTeapot(void);
	virtual ~xtDrawTeapot(void);

	virtual void Draw();
};

