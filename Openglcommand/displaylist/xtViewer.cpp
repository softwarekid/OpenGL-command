#include "StdAfx.h"
#include "xtViewer.h"

//#include <gl/GL.h>
//#include <GL.h>
#include "../../freeglut-2.6.0/include/GL/glut.h"
#include <gl/GL.h>

#include "xtCamera.h"
#include "xtModelP.h"

// constants
const float DEG2RAD = 3.141593f / 180;
const float FOV_Y = 60.0f;              // vertical FOV in degree
const float NEAR_PLANE = 1.0f;
const float FAR_PLANE = 100.0f;


xtViewer::xtViewer(void)
{
	this->matrixProjection.identity();
	this->matrixModelView.identity();
	this->drawMode = 0;

}


xtViewer::~xtViewer(void)
{
}

void xtViewer::setCamera(xtCamera *camera)
{
	this->camera = camera;
}

void xtViewer::setDrawMode(int mode)
{
	this->drawMode = mode;
}

void xtViewer::setWindowSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void xtViewer::AddModel(xtModelP *model)
{
	modellist.push_back(model);
}

void xtViewer::setFrustum(float l, float r, float b, float t, float n, float f)
{
    matrixProjection.identity();
    matrixProjection[0]  =  2 * n / (r - l);
    matrixProjection[2]  =  (r + l) / (r - l);
    matrixProjection[5]  =  2 * n / (t - b);
    matrixProjection[6]  =  (t + b) / (t - b);
    matrixProjection[10] = -(f + n) / (f - n);
    matrixProjection[11] = -(2 * f * n) / (f - n);
    matrixProjection[14] = -1;
    matrixProjection[15] =  0;
}

void xtViewer::setFrustum(float fovY, float aspectRatio, float front, float back)
{
    float tangent = tanf(fovY/2 * DEG2RAD);   // tangent of half fovY
    float height = front * tangent;           // half height of near plane
    float width = height * aspectRatio;       // half width of near plane

    // params: left, right, bottom, top, near, far
    setFrustum(-width, width, -height, height, front, back);
}

void xtViewer::setOrthoFrustum(float l, float r, float b, float t, float n/*=-1*/, float f/*f=1*/)
{
    matrixProjection.identity();
    matrixProjection[0]  =  2 / (r - l);
    matrixProjection[3]  =  -(r + l) / (r - l);
    matrixProjection[5]  =  2 / (t - b);
    matrixProjection[7]  =  -(t + b) / (t - b);
    matrixProjection[10] = -2 / (f - n);
    matrixProjection[11] = -(f + n) / (f - n);
}

///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport
///////////////////////////////////////////////////////////////////////////////
void xtViewer::setViewport(int x, int y, int w, int h)
{
    // set viewport to be the entire window
    glViewport((GLsizei)x, (GLsizei)y, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    setFrustum(FOV_Y, (float)(w)/h, NEAR_PLANE, FAR_PLANE); // FOV, AspectRatio, NearClip, FarClip

    // copy projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrixProjection.getTranspose());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void xtViewer::Draw()
{
	setViewport(0,0,windowWidth,windowHeight);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glClearColor(0.2f,0.2f,0.2f,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glPushMatrix();
	glLoadMatrixf(camera->matrixView.getTranspose());

	drawGridXY(10,1);
	drawGrid(10,1);
	drawGridYZ(10,1);

	matrixModelView = camera->matrixView*modellist[0]->GetModelMatrix();
	glLoadMatrixf(matrixModelView.getTranspose());

	drawAxis(40);
	modellist[0]->Draw();

	glPopMatrix();

	//==========================================================================
	//setViewport(0,0,windowWidth,windowHeight);


	//==========================================================================
	if(drawMode == 0)           // fill mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    else if(drawMode == 1)      // wireframe mode
    {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	else if(drawMode == 2)      // point mode
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}

}


// xz plane means the y is zero blue
///////////////////////////////////////////////////////////////////////////////
// draw a grid on the xz plane
///////////////////////////////////////////////////////////////////////////////
void xtViewer::drawGrid(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(0.0f, 0.0f, 1.0f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    //glColor3f(0.5f, 0, 0);
    //glVertex3f(-size, 0, 0);
    //glVertex3f( size, 0, 0);

    //// z-axis
    //glColor3f(0,0,0.5f);
    //glVertex3f(0, 0, -size);
    //glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

// xy plane means z is zero green/
void xtViewer::drawGridXY(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(0.0f, 1.0f, 0.0f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size,i, 0);   // lines parallel to X-axis
        glVertex3f( size,i, 0);
        glVertex3f(-size,-i,0);   // lines parallel to X-axis
        glVertex3f( size,-i,0);

        glVertex3f( i,  -size,0);   // lines parallel to Z-axis
        glVertex3f( i,   size,0);
        glVertex3f(-i,  -size,0);   // lines parallel to Z-axis
        glVertex3f(-i,   size,0);
    }

    //// x-axis
    //glColor3f(0.5f, 0, 0);
    //glVertex3f(-size, 0, 0);
    //glVertex3f( size, 0, 0);

    //// z-axis
    //glColor3f(0,0,0.5f);
    //glVertex3f(0, 0, -size);
    //glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

// x is zero Red
void xtViewer::drawGridYZ(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(0,-size,i);   // lines parallel to X-axis
        glVertex3f(0, size,i);
        glVertex3f(0,-size,-i);   // lines parallel to X-axis
        glVertex3f(0, size,-i);

        glVertex3f(0, i,  -size);   // lines parallel to Z-axis
        glVertex3f(0, i,   size);
        glVertex3f(0,-i,  -size);   // lines parallel to Z-axis
        glVertex3f(0,-i,   size);
    }

    //// x-axis
    //glColor3f(0.5f, 0, 0);
    //glVertex3f(-size, 0, 0);
    //glVertex3f( size, 0, 0);

    //// z-axis
    //glColor3f(0,0,0.5f);
    //glVertex3f(0, 0, -size);
    //glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void xtViewer::drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);
    glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
                                //      the light position when you draw GL_LINES
                                //      and GL_POINTS. remember the matrix.

    // draw axis
    glLineWidth(1);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, size);
    glEnd();
    glPointSize(10);

    // restore default settings
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}



