// displaylist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <gl/GLU.h>
#include "../../freeglut-2.6.0/include/GL/glut.h"
#include <gl/GL.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "teapot.h"
#include "Timer.h"
#include "../../txcharencodeing/txCharConvert.h"
#include "txFemSurf.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;


void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int btn, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void showInfo();
void showFPS();

// global variables
void *font = GLUT_BITMAP_8_BY_13;
GLuint listId = 0;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX = 0;
float cameraAngleY = 0;
float cameraDistance;
int drawMode = 0;
Timer timer;
bool dlUsed;

// load resource
txFemSurf femsurf;

void LoadFemSurf()
{
	femsurf.LoadFemFile("arm.fem");
}


int _tmain(int argc, _TCHAR* argv[])
{
	//char *chagv[] = new char*[argc];
	char **chargv = new char*[argc];
	for ( int i=0; i<argc; ++i ) {
		char *pchar = new char[100];
		WCharToChar(argv[i],pchar,100);
		chargv[i] = pchar;
		printf("%s\n",pchar);
		printf("%s\n",chargv[i]);
	}

	initSharedMem();

	
	initGLUT(argc,chargv);
	initGL();

	LoadFemSurf();

	listId = createTeapotDL();

	glutMainLoop();
	

	return 0;
}

bool initSharedMem()
{
	mouseLeftDown = mouseRightDown = false;
	dlUsed = true;

	return true;
}

void clearSharedMem()
{
	if (listId) {
		glDeleteLists(listId, 1); 
	}
}

int initGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );

	glutInitWindowSize(400, 300);

	glutInitWindowPosition(100, 100);

	int handle = glutCreateWindow(argv[0]);

	glutDisplayFunc(displayCB);

	glutIdleFunc(idleCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);

	return handle;
}


void initGL()
{
	glShadeModel(GL_SMOOTH);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0,0,0,0);
	glClearStencil(0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	initLights();
	setCamera(0,0,10,0,0,0);

}

void initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = {0.2f, 0.2f, 0.2f, 1.0f}; // ambient
	GLfloat lightKd[] = {.7f, .7f, .7f, .0f}; // diffuse
	GLfloat lightKs[] = {1,1,1,1}; // specular
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightKa);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightKd);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightKs);

	float lightPos[4] = {0,0,20,1};
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	glEnable(GL_LIGHT0);
}

void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX,posY,posZ,targetX,targetY,targetZ,0,1,0);
	//eye(x,y,z), focal(x,y,z),up(x,y,z);

}

void RenderSample()
{
	xtVec3df verts[3] = {xtVec3df(-1.,-1.,0),xtVec3df(1.,-1.,0),xtVec3df(0.,1.,0)};
	xtVec3df verno[3] = {xtVec3df(0.,0.,1.),xtVec3df(0.,0.,1.),xtVec3df(0.,0.,1.)};
	glBegin(GL_POLYGON);
	for ( int i=0; i<3; ++i ) {
		glNormal3f(verno[i].v[0],verno[i].v[1],verno[i].v[2]);
		glVertex3f(verts[i].v[0],verts[i].v[1],verts[i].v[2]);
	}
	glEnd();
}

void RenderSampleQuad()
{
	xtVec3df verts[4] = {xtVec3df(-1.,-1.,0),xtVec3df(1.,-1.,0),xtVec3df(1.,1.,0),xtVec3df(-1.,1.,0)};
	xtVec3df verno[4] = {xtVec3df(0.,0.,1.),xtVec3df(0.,0.,1.),xtVec3df(0.,0.,1.),xtVec3df(0.,0.,1.)};
	glBegin(GL_POLYGON);
	for ( int i=0; i<4; ++i ) {
		glNormal3f(verno[i].v[0],verno[i].v[1],verno[i].v[2]);
		glVertex3f(verts[i].v[0],verts[i].v[1],verts[i].v[2]);
	}
	glEnd();
}

void displayCB()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();

	glTranslatef(0,0,cameraDistance);
	glRotatef(cameraAngleX,1,0,0);
	glRotatef(cameraAngleY,0,1,0);

	timer.start();

	if(dlUsed)
		glCallList(listId);
	else
		//drawTeapot();
		//femsurf.Draw();
		//femsurf.RenderSurf2();
		//RenderSample();
		RenderSampleQuad();
	setCamera(2000,2000,2000,2000,2000,2000);

	timer.stop();

	showInfo();
	showFPS();

	glPopMatrix();
	glutSwapBuffers();

}

void reshapeCB(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);

	float aspectRatio = (float)w/h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f,(float)(w)/h,1.0f,1000.0f);

	glMatrixMode(GL_MODELVIEW);
}

void timerCB(int millisec)
{
	glutTimerFunc(millisec,timerCB,millisec);
	glutPostRedisplay();
}

void idleCB()
{
	glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		clearSharedMem();
		exit(0);
		break;
	case 'd':
	case 'D':
		drawMode = ++drawMode%3;
		if (drawMode == 0) {
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
		} else if(drawMode == 1) {
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		break;
	case ' ':
		dlUsed = !dlUsed;
		break;
	default:
		;
	}
	glutPostRedisplay();
}

void mouseCB(int btn, int stat, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (btn == GLUT_LEFT_BUTTON) {
		if (stat==GLUT_DOWN) {
			mouseLeftDown = true;
		} else if (stat==GLUT_UP) {
			mouseLeftDown = false;
		}
	} else if (btn==GLUT_RIGHT_BUTTON) {
		if (stat==GLUT_DOWN) {
			mouseRightDown = true;
		} else if (stat == GLUT_UP) {
			mouseRightDown = false;
		}
	}
}

void mouseMotionCB(int x, int y)
{
	if (mouseLeftDown) {
		cameraAngleX +=(x-mouseX);
		cameraAngleY +=(y-mouseY);
		mouseX = x;
		mouseY = y;
	}

	if (mouseRightDown) {
		cameraDistance += (y-mouseY)*0.2f;
		mouseY = y;
	}

	glutPostRedisplay();
}

void showInfo()
{
	glPushMatrix();					// save current modelview matrix
	glLoadIdentity();				// reset modelview matrix

	glMatrixMode(GL_PROJECTION);	// switch to projection matrix
	glPushMatrix();					// save current project matrix
	glLoadIdentity();				// reset projection matrix
	gluOrtho2D(0,400,0,300);		// set to orthogonal projection

	float color[4] = {1,1,1,1};

	std::stringstream ss;
	ss<<std::fixed<<std::setprecision(3);

	ss << "Display List:" << (dlUsed ? "on" : "off") <<ends;
	drawString(ss.str().c_str(),1,286,color,font);
	ss.str("");

	ss << "Time: " << timer.getElapsedTimeInMicroSec() << " ms" << ends;
	drawString(ss.str().c_str(),1,1,color,font);

	ss<<std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);



    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix

}

void showFPS()
{
	static Timer timer;
	static int count = 0;
	static stringstream ss;
	double elapsedTime;

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,400,0,300);

	float color[4] = {1,1,0,1};

	elapsedTime = timer.getElapsedTime();

	if (elapsedTime <1.0) {
		++count;
	} else {
		ss.str("");
		ss << std::fixed<<std::setprecision(1);
		ss << (count/elapsedTime) << " FPS" << ends;
		ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		count = 0;
		timer.start();
	}
	drawString(ss.str().c_str(), 315, 286, color, font);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawString(const char *str, int x, int y, float color[4], void *font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);

	glColor4fv(color);
	glRasterPos2i(x,y);

	while(*str) {
		glutBitmapCharacter(font,*str);
		++str;
	}

	glEnable(GL_LIGHTING);
	glPopAttrib();
}


