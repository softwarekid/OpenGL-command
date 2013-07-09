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

void xtFemViewer::draw(const std::string& _draw_mode)
{
  if (!femsurf)
  {
    xtGlutControl::draw(_draw_mode);
    return;
  }

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
  }


}
