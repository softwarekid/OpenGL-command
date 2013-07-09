#include "StdAfx.h"
#include "xtDrawTeapot.h"

#include "../../freeglut-2.6.0/include/GL/glut.h"
#include <gl/GL.h>

#include "teapot.h"


xtDrawTeapot::xtDrawTeapot(void)
{
}


xtDrawTeapot::~xtDrawTeapot(void)
{
}

void xtDrawTeapot::Draw()
{
	drawTeapot();
}
