#include "StdAfx.h"
#include "xtGlutControl.h"
#include <cmath>

//#include "xtPickingUtil.h"

#ifndef M_PI
  #define M_PI      3.14159265359
#endif

//xtGlutControl::xtGlutControl(void)
//{
//}

xtGlutControl::xtGlutControl(const char *title, int width, int height)
	:xtGlutView(title,width,height)
{
	init();

	for ( int i=0; i<10; ++i ) {
		button_down_[i] = false;
	}

  // draw mode
  add_draw_mode("Wireframe");
  add_draw_mode("Hidden Line");
  add_draw_mode("Solid Flat");
  add_draw_mode("Solid Smooth");
  set_draw_mode(1);

  // set pick true
  picksw = true;
}


xtGlutControl::~xtGlutControl(void)
{
}

void 
xtGlutControl::init()
{
  // OpenGL state
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glDisable( GL_DITHER );
  glEnable( GL_DEPTH_TEST );


  // some performance settings
//   glEnable( GL_CULL_FACE );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );


  // material
  GLfloat mat_a[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat mat_d[] = {0.4f, 0.4f, 0.4f, 1.0f};
  GLfloat mat_s[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat shine[] = {128.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_a);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_d);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_s);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

  
  // lighting
  glLoadIdentity();
  
  GLfloat pos1[] = { 0.1f, 0.1f, -0.02f, 0.0f};
  GLfloat pos2[] = {-0.1f, 0.1f, -0.02f, 0.0f};
  GLfloat pos3[] = { 0.0f, 0.0f, 0.1f, 0.0f};
  GLfloat col1[] = {.05f, .05f, .6f, 1.0f};
  GLfloat col2[] = {.6f, .05f, .05f, 1.0f};
  GLfloat col3[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glEnable(GL_LIGHT0);    
  glLightfv(GL_LIGHT0,GL_POSITION, pos1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,  col1);
  glLightfv(GL_LIGHT0,GL_SPECULAR, col1);
  
  glEnable(GL_LIGHT1);  
  glLightfv(GL_LIGHT1,GL_POSITION, pos2);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,  col2);
  glLightfv(GL_LIGHT1,GL_SPECULAR, col2);
  
  glEnable(GL_LIGHT2);  
  glLightfv(GL_LIGHT2,GL_POSITION, pos3);
  glLightfv(GL_LIGHT2,GL_DIFFUSE,  col3);
  glLightfv(GL_LIGHT2,GL_SPECULAR, col3);
  

  // scene pos and size
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
  set_scene(Vector3(0.0, 0.0, 0.0), 1.0);


  // projection
  near_ = 0.1f;
  far_  = 100.0f;
  fovy_ = 45.0f;
}

void
xtGlutControl::set_scene( const Vector3& _cog, float _radius )
{
  center_ = _cog;
  radius_ = _radius;

  near_  = 0.01f * radius_;
  far_   = 10.0f * radius_;
  update_projection_matrix();

  view_all();
}


//----------------------------------------------------------------------------


void
xtGlutControl::view_all()
{  
  translate( Vector3( -(modelview_matrix_[0]*center_[0] + 
		      modelview_matrix_[4]*center_[1] +
		      modelview_matrix_[8]*center_[2] + 
		      modelview_matrix_[12]),
		    -(modelview_matrix_[1]*center_[0] + 
		      modelview_matrix_[5]*center_[1] +
		      modelview_matrix_[9]*center_[2] + 
		      modelview_matrix_[13]),
		    -(modelview_matrix_[2]*center_[0] + 
		      modelview_matrix_[6]*center_[1] +
		      modelview_matrix_[10]*center_[2] + 
		      modelview_matrix_[14] +
		      3.0*radius_) ) );
}

void
xtGlutControl::translate( const Vector3& _trans )
{
  glLoadIdentity();
  glTranslated( _trans[0], _trans[1], _trans[2] );
  glMultMatrixd( modelview_matrix_ );
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix_);
}

void
xtGlutControl::update_projection_matrix()
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective(fovy_, (GLfloat)m_Width/(GLfloat)m_Height, near_, far_);
  glGetDoublev( GL_PROJECTION_MATRIX, projection_matrix_);
  glMatrixMode( GL_MODELVIEW );
}

bool xtGlutControl::map_to_sphere( const Vec2i& _v2D, Vector3& _v3D )
{
  if ( (_v2D[0] >= 0) && (_v2D[0] <= m_Width) &&
       (_v2D[1] >= 0) && (_v2D[1] <= m_Height) ) 
  {
    double x  = (double)(_v2D[0] - 0.5*m_Width)  / (double)m_Width;
    double y  = (double)(0.5*m_Height - _v2D[1]) / (double)m_Height;
    double sinx         = sin(M_PI * x * 0.5  );
    double siny         = sin(M_PI * y * 0.5);
    double sinx2siny2   = sinx * sinx + siny * siny;
    
    _v3D[0] = sinx;
    _v3D[1] = siny;
    _v3D[2] = sinx2siny2 < 1.0 ? sqrt(1.0 - sinx2siny2) : 0.0;
    
    return true;
  }
  else return false;
}

void 
xtGlutControl::draw(const std::string& _draw_mode)
{
  if (_draw_mode == "Wireframe")
  {
    glDisable(GL_LIGHTING);
    glutWireTeapot(0.5);
  }

  else if (_draw_mode == "Solid Flat")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glutSolidTeapot(0.5);
  }

  else if (_draw_mode == "Solid Smooth")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glutSolidTeapot(0.5);
  }
  else if (_draw_mode == "Hidden Line")
  {
	  glDisable(GL_LIGHTING);
	  glShadeModel(GL_SMOOTH);
	  glColor3f(0.0, 0.0, 0.0);

	  glDepthRange(0.01, 1.0);
	  glutSolidTeapot(0.5);

	  glColor3f(1.0, 1.0, 1.0);
	  glDepthRange(0.0, 1.0);
	  glutWireTeapot(0.5);
  } 
  else 
  {
	  std::cout << "This view mode is not supported for this geometry, you need to load a mesh!" << std::endl;
  }
}


void 
xtGlutControl::mouse(int button, int state, int x, int y)
{
  // mouse press
  if (state == GLUT_DOWN)
  {
    last_point_2D_ = Vec2i(x,y);
    last_point_ok_ = map_to_sphere( last_point_2D_, last_point_3D_ );
    button_down_[button] = true;

	if ( picksw ) {
		scweenxy = Vec2i(x,y);
		//GetRay(x,y,m_Width,m_Height,fovy_,near_,far_,projection_matrix_,modelview_matrix_,ray);
		GetRay2(x,y,m_Width,m_Height,fovy_,near_,far_,projection_matrix_,modelview_matrix_,ray);
		printf("ray:%f,%f,%f<=>%f,%f,%f\n",ray.orin.x,ray.orin.y,ray.orin.z,ray.dir.x,ray.dir.y,ray.dir.z);
	}
  }


  // mouse release
  else
  {
    last_point_ok_ = false;
    button_down_[button] = false;

    // GLUT: button 3 or 4 -> mouse wheel clicked
    if (button == 3)       
      zoom(0, (int)(last_point_2D_[1] - 0.05*m_Width));
    else if (button == 4)
      zoom(0, (int)(last_point_2D_[1] + 0.05*m_Height));
  }


  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
xtGlutControl::motion(int x, int y)
{
  // zoom
  if (button_down_[0] && button_down_[1])
  {
    zoom(x, y);
  }

  // rotation
  else if (button_down_[0])
  {
    rotation(x, y);
  }

  // translation
  else if (button_down_[1])
  {
    translation(x, y);
  }


  // remeber points
  last_point_2D_ = Vec2i(x, y);
  last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);

  glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void 
xtGlutControl::rotation(int x, int y)
{
  if (last_point_ok_) 
  {
    Vec2i  new_point_2D;
    Vector3  new_point_3D;
    bool   new_point_ok;

    new_point_2D = Vec2i(x, y);
    new_point_ok = map_to_sphere(new_point_2D, new_point_3D);
    
    if (new_point_ok)
    {
		Vector3 axis      = (last_point_3D_.cross(new_point_3D));
		float cos_angle = (last_point_3D_.dot( new_point_3D ) );

      if (fabs(cos_angle) < 1.0) 
      {
	float angle = 2.0*acos(cos_angle) * 180.0 / M_PI;
	rotate(axis, angle);
      }
    }
  }
}


//-----------------------------------------------------------------------------


void 
xtGlutControl::translation(int x, int y)
{
  float dx = x - last_point_2D_[0];
  float dy = y - last_point_2D_[1];

  float z = - ((modelview_matrix_[ 2]*center_[0] + 
		modelview_matrix_[ 6]*center_[1] + 
		modelview_matrix_[10]*center_[2] + 
		modelview_matrix_[14]) /
	       (modelview_matrix_[ 3]*center_[0] + 
		modelview_matrix_[ 7]*center_[1] + 
		modelview_matrix_[11]*center_[2] + 
		modelview_matrix_[15]));

  float aspect = (float)m_Width / (float)m_Height;
  float up     = tan(fovy_/2.0f*M_PI/180.f) * near_;
  float right  = aspect*up;

  translate(Vector3(2.0*dx/m_Width*right/near_*z, 
		  -2.0*dy/m_Height*up/near_*z, 
		  0.0f));
}


//-----------------------------------------------------------------------------


void 
xtGlutControl::zoom(int x, int y)
{
  float dy = y - last_point_2D_[1];
  float h  = m_Height;
  translate(Vector3(0.0, 0.0, radius_ * dy * 3.0 / h));
}


//----------------------------------------------------------------------------


//void
//xtGlutControl::translate( const Vector3& _trans )
//{
//  glLoadIdentity();
//  glTranslated( _trans[0], _trans[1], _trans[2] );
//  glMultMatrixd( modelview_matrix_ );
//  glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix_);
//}


//----------------------------------------------------------------------------


void
xtGlutControl::rotate( const Vector3& _axis, float _angle )
{
  Vector3 t( modelview_matrix_[0]*center_[0] + 
	   modelview_matrix_[4]*center_[1] +
	   modelview_matrix_[8]*center_[2] + 
	   modelview_matrix_[12],
	   modelview_matrix_[1]*center_[0] + 
	   modelview_matrix_[5]*center_[1] +
	   modelview_matrix_[9]*center_[2] + 
	   modelview_matrix_[13],
	   modelview_matrix_[2]*center_[0] + 
	   modelview_matrix_[6]*center_[1] +
	   modelview_matrix_[10]*center_[2] + 
	   modelview_matrix_[14] );
  
  glLoadIdentity();
  glTranslatef(t[0], t[1], t[2]);
  glRotated( _angle, _axis[0], _axis[1], _axis[2]);
  glTranslatef(-t[0], -t[1], -t[2]); 
  glMultMatrixd(modelview_matrix_);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
}


//-----------------------------------------------------------------------------


void 
xtGlutControl::keyboard(int key, int x, int y) 
{
  switch (key)
  {
    case 'f':
    {
      std::cerr << "Performance test: ";
      double fps = measure_fps();
      std::cerr << fps << " FPS\n";
      break;
    }

    default:
    {
      xtGlutView::keyboard(key, x, y);
      break;
    }
  }
}

double 
xtGlutControl::measure_fps()
{
  double fps(0.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  unsigned int  frames = 90;
  const float   angle  = 360.0/(float)frames;
  unsigned int  i;
  //Vec3f         axis;

  //OpenMesh::Utils::Timer timer; timer.start();
	
  //for (i=0, axis=Vec3f(1,0,0); i<frames; ++i)
  //{ rotate(axis, angle); display(); }
  //for (i=0, axis=Vec3f(0,1,0); i<frames; ++i)
  //{ rotate(axis, angle); display(); }
  //for (i=0, axis=Vec3f(0,0,1); i<frames; ++i)
  //{ rotate(axis, angle); display(); }

  glFinish();

  //timer.stop();
  fps = 1000; /*(1000.0 / timer.mseconds() * (3.0 * frames));*/

  glPopMatrix();
  glutPostRedisplay();

  return fps;
}

void
xtGlutControl::reshape(int _w, int _h)
{
  m_Width  = _w; 
  m_Height = _h;
  glViewport(0, 0, _w, _h);
  update_projection_matrix();
  glutPostRedisplay();
}

void xtGlutControl::DrawRay()
{
	//glTranslate(
	glutSolidTeapot(1);

	float start[3] = {ray.orin.x,ray.orin.y,ray.orin.z};
	const float scale = 100;
	Vector3 endv(ray.orin + scale*ray.dir);
	Vector3 startpa(ray.orin -scale*ray.dir);
	float end[3] = {endv.x,endv.y,endv.z};
	glDisable(GL_LIGHTING);

	glBegin(GL_LINE);
	glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(start[0],start[1],start[2]);
		glVertex3f(end[0],end[1],end[2]);
	glEnd();

	

	glPointSize(10);
	glBegin(GL_POINTS);
		glColor3f(0.0f,0.0f,1.0f);
		for ( double delta=-1000.; delta<0; delta+=5.  ) {
			//glVertex3f(end[0],end[1],end[2]);
			Vector3 drawPnt = ray.orin + delta*ray.dir;
			glVertex3f(drawPnt.x,drawPnt.y,drawPnt.z);
		}
		glColor3f(1.0f,0.0f,0.0f);
		for ( double delta=0; delta<1000; delta+=5.  ) {
			//glVertex3f(end[0],end[1],end[2]);
			Vector3 drawPnt = ray.orin + delta*ray.dir;
			glVertex3f(drawPnt.x,drawPnt.y,drawPnt.z);
		}
		
	glEnd();

	glEnable(GL_LIGHTING);
}

//BLUE
void xtGlutControl::drawGrid(float size, float step)
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
void xtGlutControl::drawGridXY(float size, float step)
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
void xtGlutControl::drawGridYZ(float size, float step)
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








