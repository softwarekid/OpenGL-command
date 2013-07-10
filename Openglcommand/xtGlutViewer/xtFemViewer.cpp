#include "StdAfx.h"
#include "xtFemViewer.h"
#include "txFemSurf.h"


xtFemViewer::xtFemViewer(const char *title, int width, int height)
	:xtGlutControl(title,width,height)
{
	this->femsurf = new txFemSurf;

  clear_draw_modes();
  add_draw_mode("Wireframe");
  add_draw_mode("Hidden Line");
  add_draw_mode("Solid Flat");
  add_draw_mode("Solid Smooth");
  set_draw_mode(3);
  isHit = false;
}


xtFemViewer::~xtFemViewer(void)
{
}

void xtFemViewer::LoadFem(const char *filename)
{
	this->femsurf->LoadFemFile(filename);
	xtBBox bbox = this->femsurf->GetBBox();

	set_scene(Vector3(bbox.center.v[0],bbox.center.v[1],bbox.center.v[2]),bbox.extend.Length());

}


// over write the xtGlutControl's mouse event to pick
void xtFemViewer::mouse(int button, int state, int x, int y)
{
  // mouse press
  if (state == GLUT_DOWN)
  {
    last_point_2D_ = Vec2i(x,y);
    last_point_ok_ = map_to_sphere( last_point_2D_, last_point_3D_ );
    button_down_[button] = true;

	if ( picksw && button!=3 && button!=4 ) {
		scweenxy = Vec2i(x,y);
		//GetRay(x,y,m_Width,m_Height,fovy_,near_,far_,projection_matrix_,modelview_matrix_,ray);
		//GetRay2(x,y,m_Width,m_Height,fovy_,near_,far_,projection_matrix_,modelview_matrix_,ray);
		GetRay3(x,y,m_Width,m_Height,fovy_,near_,far_,projection_matrix_,modelview_matrix_,ray);
		printf("::ray:%f,%f,%f<=>%f,%f,%f\n",ray.orin.x,ray.orin.y,ray.orin.z,ray.dir.x,ray.dir.y,ray.dir.z);
		isHit = femsurf->RayCast(&ray,hitElementType,hitElementIndex);
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

void xtFemViewer::draw(const std::string& _draw_mode)
{
  if (!femsurf)
  {
    xtGlutControl::draw(_draw_mode);
    return;
  }

  xtBBox bbox = femsurf->GetBBox();
  double length = bbox.extend.Length();
  length *=3.;
  drawGridXY(length,length/10.);
  drawGrid(length,length/10.);
  drawGridYZ(length,length/10.);

  if (_draw_mode == "Wireframe")
  {
	  glDisable(GL_LIGHTING);
	  femsurf->RenderSurf(true);
  }


  else if (_draw_mode == "Hidden Line")
  {



  }


  else if (_draw_mode == "Solid Flat")
  {

  }


  else if (_draw_mode == "Solid Smooth")
  {
	  glDisable(GL_LIGHTING);
	  femsurf->RenderSurf2();

	  DrawRay();

	  if ( isHit ) {
		  femsurf->DrawHit(this->hitElementType,this->hitElementIndex);
	  }
  }


}
