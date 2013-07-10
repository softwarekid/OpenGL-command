#pragma once
#include "Vectors.h"

#include <math.h>
#include <cmath>

#include "Matrices.h"


#ifndef M_PI
  #define M_PI      3.14159265359
#endif


class xtPickingUtil
{
public:
	xtPickingUtil(void);
	~xtPickingUtil(void);
};

/**
> position is a 3d vector (Vector3f) holding the world position of the camera
> view is a 3d vector (Vector3f) holding the viewing direction of the camera
> pickingRay is the helper class given above, basically it is a line with starting 
  point PickingRay.clickPosInWorld and a direction PickingRay.direction. 
  You need to instantiate it.
> two scalars viewportWidth and viewportHeight being the width and height of 
  the current view port.
> two scalars screenX and screenY are the mouse coordinates on your 2D screen 
  (origin in lower left corner)
*/


struct xtRaypick
{
	Vector3 orin;
	Vector3 dir;
};

inline void Inversevector( Vector3 &v, Matrix4 &m)
{
	v = Vector3(
		m[0]*v[0]+
		m[4]*v[1]+
		m[8]*v[2]+
		m[12],

		m[1]*v[0]+
		m[5]*v[1]+
		m[9]*v[2]+
		m[13],

		m[2]*v[0]+
		m[6]*v[1]+
		m[10]*v[2]+
		m[14]);
};

inline Vector4 InverseVector4( Vector4 &v, Matrix4 &m )
{
	Vector4 rlt;
	rlt = Vector4(
		m[0]*v[0]+
		m[4]*v[1]+
		m[8]*v[2]+
		m[12]*v[3],

		m[1]*v[0]+
		m[5]*v[1]+
		m[9]*v[2]+
		m[13]*v[3],

		m[2]*v[0]+
		m[6]*v[1]+
		m[10]*v[2]+
		m[14]*v[3],

		m[3]*v[0]+
		m[7]*v[1]+
		m[11]*v[2]+
		m[15]*v[3]
		);

		return rlt;
};

inline Vector4 InverseVector4OnlyTran( Vector4 &v, Matrix4 &m )
{
	Vector4 rlt;
	rlt = Vector4(
		m[0]*v[0]+
		m[4]*v[1]+
		m[8]*v[2],

		m[1]*v[0]+
		m[5]*v[1]+
		m[9]*v[2],

		m[2]*v[0]+
		m[6]*v[1]+
		m[10]*v[2],

		0.0
		);

	return rlt;
}


inline void SelfInverseVector4( Vector4 &v, Matrix4 &m )
{
	v = InverseVector4(v,m);
};



//http://www.antongerdelan.net/opengl/raycasting.html
// There're 6 stage from the model space to the screen space
// > 0 model space
// > 1 world space
// > 2 view  space
// > 3 homogeneouse clipping space
// > 4 nomalized device space x, y, z [-1.0,1.0]
// > 5 viewport space

inline void GetRay2(int mouseX, int mouseY, int width, int height,
	const double fovy, const double raynear, const double far,
	double *projection_matrix, double *modelview_matrix, xtRaypick &ray)
{
	// inverse projection & model_veiw matrix
	//=======================================================================================
	Matrix4 projectm;
	Matrix4 modelviewm;
	modelviewm.identity();
	projectm.identity();
	float projectmf[16];
	float modelviewf[16];
	for ( int i=0; i<16; ++i ) {
		projectmf[i] = (float) projection_matrix[i];
		modelviewf[i] = (float) modelview_matrix[i];
	}

	projectm.setColumn(0,&(projectmf[0]));
	projectm.setColumn(1,&(projectmf[4]));
	projectm.setColumn(2,&(projectmf[8]));
	projectm.setColumn(3,&(projectmf[12]));

	modelviewm.setColumn(0,&(modelviewf[0]));
	modelviewm.setColumn(1,&(modelviewf[4]));
	modelviewm.setColumn(2,&(modelviewf[8]));
	modelviewm.setColumn(3,&(modelviewf[12]));

	Matrix4 invprojectm = projectm.invert();
	Matrix4 invmodelviewm = modelviewm.invert();
	Matrix4 invprojectmtran;
	Matrix4 invmodelviewmtran;
	const float *inproject = invprojectm.getTranspose();
	const float *invmodelt = invmodelviewm.getTranspose();
	for ( int i=0; i<16; i++ ) {
		invprojectmtran[i] = inproject[i];
		invmodelviewmtran[i] = invmodelt[i];
	}
	//=======================================================================================



	// view port sceen space => normalized device space
	//=======================================================================================
	const double normalizedDeviceX = 2.0*mouseX/width -1.0;
	const double normalizedDeviceY = 1.0 - 2.0*mouseY/height;
	const double normalizedDeviceZ = 1.0;

	Vector3 ray_normalizedDevice(normalizedDeviceX,normalizedDeviceY,normalizedDeviceZ);

	// normalized device space => homogeneouse clip coordinate
	//=======================================================================================
	Vector4 ray_hclip(ray_normalizedDevice.x,ray_normalizedDevice.y,1.0,1.0);

	// homogeneouse clip coordinate => 4d Eye (Camera) coordinate
	//=======================================================================================
	Vector4 ray_eye = InverseVector4(ray_hclip,invprojectmtran);
	//Vector4 ray_eye = InverseVector4(ray_hclip,invprojectm);
	Vector4 ray_orin(0.0,0.0,0.0,1.0);
	

	// to model space
	//=======================================================================================
	Vector4 ray_model = InverseVector4(ray_eye,invmodelviewmtran);
	Vector4 eye_orin_model = InverseVector4(ray_orin,invmodelviewmtran);
	//Vector4 ray_model = InverseVector4(ray_eye,invmodelviewm);
	//Vector4 eye_orin_model = InverseVector4(ray_orin,invmodelviewm);

	Vector3 orinv3(eye_orin_model.x,eye_orin_model.y,eye_orin_model.z);
	Vector3 ray_model_normal(ray_model.x, ray_model.y, ray_model.z);
	ray_model_normal.normalize();
	ray.dir = ray_model_normal;
	ray.orin = orinv3;
}


// idear frm the D3D10 sample Pick10
inline void GetRay3(int mouseX, int mouseY, int width, int height,
	const double fovy, const double raynear, const double far,
	double *projection_matrix, double *modelview_matrix, xtRaypick &ray)
{
	// inverse projection & model_veiw matrix
	//=======================================================================================
	Matrix4 projectm;
	Matrix4 modelviewm;
	modelviewm.identity();
	projectm.identity();
	float projectmf[16];
	float modelviewf[16];
	for ( int i=0; i<16; ++i ) {
		projectmf[i] = (float) projection_matrix[i];
		modelviewf[i] = (float) modelview_matrix[i];
	}

	projectm.setColumn(0,&(projectmf[0]));
	projectm.setColumn(1,&(projectmf[4]));
	projectm.setColumn(2,&(projectmf[8]));
	projectm.setColumn(3,&(projectmf[12]));

	modelviewm.setColumn(0,&(modelviewf[0]));
	modelviewm.setColumn(1,&(modelviewf[4]));
	modelviewm.setColumn(2,&(modelviewf[8]));
	modelviewm.setColumn(3,&(modelviewf[12]));

	Matrix4 invprojectm = projectm.invert();
	Matrix4 invmodelviewm = modelviewm.invert();
	Matrix4 invprojectmtran;
	Matrix4 invmodelviewmtran;
	const float *inproject = invprojectm.getTranspose();
	const float *invmodelt = invmodelviewm.getTranspose();
	for ( int i=0; i<16; i++ ) {
		invprojectmtran[i] = inproject[i];
		invmodelviewmtran[i] = invmodelt[i];
	}
	//=======================================================================================



	// view port sceen space => normalized device space
	//=======================================================================================
	const double normalizedDeviceX = 2.0*mouseX/width -1.0;
	const double normalizedDeviceY = 1.0 - 2.0*mouseY/height;
	const double normalizedDeviceZ = 1.0;

	Vector3 ray_normalizedDevice(normalizedDeviceX,normalizedDeviceY,normalizedDeviceZ);

	// normalized device space => homogeneouse clip coordinate
	//=======================================================================================
	Vector4 ray_hclip(ray_normalizedDevice.x,ray_normalizedDevice.y,1.0,1.0);

	// homogeneouse clip coordinate => 4d Eye (Camera) coordinate
	//=======================================================================================
	//Vector4 ray_eye = InverseVector4(ray_hclip,invprojectmtran);
	Vector4 ray_eye(ray_normalizedDevice.x/projection_matrix[0],ray_normalizedDevice.y/projection_matrix[5],-1.0,1.0);
	//Vector4 ray_eye = InverseVector4(ray_hclip,invprojectm);
	Vector4 ray_orin(0.0,0.0,0.0,1.0);
	

	// to model space
	//=======================================================================================
	//Vector4 ray_model = InverseVector4(ray_eye,invmodelviewmtran);
	Vector4 ray_model = InverseVector4OnlyTran(ray_eye,invmodelviewmtran);
	//Vector4 eye_orin_model = InverseVector4(ray_orin,invmodelviewmtran);
	Vector4 eye_orin_model(invmodelviewmtran[12],invmodelviewmtran[13],invmodelviewmtran[14],0.0);
	//Vector4 ray_model = InverseVector4(ray_eye,invmodelviewm);
	//Vector4 eye_orin_model = InverseVector4(ray_orin,invmodelviewm);

	Vector3 orinv3(eye_orin_model.x,eye_orin_model.y,eye_orin_model.z);
	Vector3 ray_model_normal(ray_model.x, ray_model.y, ray_model.z);
	ray_model_normal.normalize();
	ray.dir = ray_model_normal;
	ray.orin = orinv3;
}


inline void GetRay(int mouseX, int mouseY, int width, int height,
	const double fovy, const double raynear, const double far,
	double *projection_matrix, double *modelview_matrix, xtRaypick &ray)
{
	// cs mean camera space
	//const double csSceenZ = near;
	const double csSceenZ = raynear;
	const double csSceenHalfHeight = csSceenZ*tan(fovy*M_PI/(2*180.0));
	const double csSceenHalfWidth  = ((double)width/height)*csSceenHalfHeight;

	const double csSceenX = (mouseX-0.5*width)/(0.5*width)*csSceenHalfWidth;
	const double csSceenY = (0.5*height-mouseY)/(0.5*height)*csSceenHalfHeight;

	Matrix4 projectm;
	Matrix4 modelviewm;
	modelviewm.identity();
	projectm.identity();
	float projectmf[16];
	float modelviewf[16];
	for ( int i=0; i<16; ++i ) {
		projectmf[i] = (float) projection_matrix[i];
		modelviewf[i] = (float) modelview_matrix[i];
	}

	projectm.setColumn(0,&(projectmf[0]));
	projectm.setColumn(1,&(projectmf[4]));
	projectm.setColumn(2,&(projectmf[8]));
	projectm.setColumn(3,&(projectmf[12]));

	modelviewm.setColumn(0,&(modelviewf[0]));
	modelviewm.setColumn(1,&(modelviewf[4]));
	modelviewm.setColumn(2,&(modelviewf[8]));
	modelviewm.setColumn(3,&(modelviewf[12]));

	Matrix4 invprojectm = projectm.invert();
	Matrix4 invmodelviewm = modelviewm.invert();

	// project the camera original point and the sceen point 
	// to the model framwork!
	Vector3 orin(0.0,0.0,0.0);
	Inversevector(orin,invprojectm);
	Inversevector(orin,invmodelviewm);

	Vector3 dest(csSceenX,csSceenY,csSceenZ);
	Inversevector(dest,invprojectm);
	Inversevector(dest,invmodelviewm);

	ray.orin = orin;
	ray.dir = (dest-orin).normalize();

};
