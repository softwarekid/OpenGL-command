// xtGlutViewer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "xtGlutView.h"
#include "xtGlutControl.h"
#include "xtFemViewer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//xtGlutView viewer("Hello GLUT",400,300);
	//xtGlutControl viewer("Hello GLUT",400,300);

	xtFemViewer femviewer("Mesh Displayer",400,300);
	femviewer.LoadFem("arm.fem");

	glutMainLoop();
	return 0;
}

