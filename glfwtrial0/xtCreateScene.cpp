#include "StdAfx.h"
#include "xtCreateScene.h"
#include "xtVertexBufferObject.h"
#include "xtShader.h"
/**
 One VBO, where all static data are stored now,
 in this tutorial vertex is stored as 3 floats for
 postion and 2 floats for texture coordinates
*/

xtVertexBufferObject vboSceneObjects;
unsigned int uiVAO;

xtShader shVertex, shFragment;
xtShaderProgram spMain;

xtShader tGold, tSnow;



xtCreateScene::xtCreateScene(void)
{
}


xtCreateScene::~xtCreateScene(void)
{
}

float points[] = {
   -0.5f,  -0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
   0.5f, 0.5f,  0.0f,
   -0.5f,  -0.5f,  0.0f,
    0.5f, 0.5f,  0.0f,
   -0.5f, 0.5f,  0.0f
};

float texcoords[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	0.0f,1.0f,
};

void initScene(xtLPVOID)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	vboSceneObjects.CreateVBO();
	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);

	vboSceneObjects.BindVBO();

	for ( int i=0; i<6; ++i ) {
		vboSceneObjects.AddData( &points[i], 3*sizeof(float) );
		vboSceneObjects.AddData( &texcoords[i], 2*sizeof(float));
	}

	vboSceneObjects.UpLoadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE,sizeof(float)*5,(void*)(3*sizeof(float)));

}

void renderScene(xtLPVOID)
{

}

void releaseScene(xtLPVOID)
{
	
}
