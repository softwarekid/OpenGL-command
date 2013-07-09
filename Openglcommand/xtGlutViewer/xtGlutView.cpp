#include "StdAfx.h"
#include "xtGlutView.h"


//xtGlutView::xtGlutView(void)
//{
//}

xtGlutView * xtGlutView::m_Window = NULL;

xtGlutView::xtGlutView(const char *title, int width, int height)
	: m_Width(width), m_Height(height)
{
	int argc = 1;
	char *argv[] = {"hello"};
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(m_Width, m_Height);
	glutInitWindowPosition(100, 100);
	m_WindowId = glutCreateWindow(title);

	m_Window = this;

  // register callbacks
  glutDisplayFunc(display__);
  glutKeyboardFunc(keyboard__);
  glutSpecialFunc(special__);
  glutMouseFunc(mouse__);
  glutMotionFunc(motion__);
  glutPassiveMotionFunc(passivemotion__);
  glutReshapeFunc(reshape__); 
  glutVisibilityFunc(visibility__);
  //glutMouseWheelFunc(mouseWheel__);

  // create menu
  this->m_NumDrawMode = 0;
  this->m_MenuID = glutCreateMenu(processmenu__);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

}


xtGlutView::~xtGlutView(void)
{
  

  glutDestroyWindow(m_WindowId);
  glutDestroyMenu(m_MenuID);
}

void xtGlutView::clear_draw_modes()
{
	for ( unsigned int i=0; i<this->m_NumDrawMode; ++i ) {
		glutRemoveMenuItem(1);
	}

	this->m_NumDrawMode = 0;
	this->m_DrawModelNames.clear();
}

unsigned int
xtGlutView::add_draw_mode(const std::string& _s)
{
  // insert in popup menu
  glutAddMenuEntry(_s.c_str(), m_NumDrawMode);

  ++m_NumDrawMode;
  m_DrawModelNames.push_back(_s);

  return m_NumDrawMode-1;
}

void
xtGlutView::set_draw_mode(int _id)
{
	this->m_DrawMode = _id;
  glutPostRedisplay();
}


xtGlutView *xtGlutView::Get_Window()
{
	return m_Window;
}

void xtGlutView::keyboard(int key, int x, int y) 
{
  switch (key)
  {
    case 27:  
    {
      exit(0); 
      break;
    }


    case GLUT_KEY_F12: 
    {
      if (!m_Fullscreen) 
      {
	m_BakLeft   = glutGet(GLUT_WINDOW_X);
	m_BakTop    = glutGet(GLUT_WINDOW_Y);
	mBakWidth  = glutGet(GLUT_WINDOW_WIDTH);
	mBakHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glutFullScreen();
	m_Fullscreen = true;
      }
      else
      {
	glutReshapeWindow(mBakWidth, mBakHeight);
	glutPositionWindow(m_BakLeft, m_BakTop);
	m_Fullscreen = false;
      }
      break;
    }
  }
} 


void xtGlutView::display(void) 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_DrawMode < m_DrawModelNames.size())
    draw(m_DrawModelNames[m_DrawMode]);
  else
    draw("");

  glutSwapBuffers();
}



//=======================================================================
// Cheat the compiler
//void xtGlutView::display(void) {}
void xtGlutView::idle(void) {}
//void xtGlutView::keyboard(int key, int x, int y) {}
void xtGlutView::motion(int x, int y) {}
void xtGlutView::mouse(int button, int state, int x, int y) {}
void xtGlutView::passivemotion(int x, int y) {}
void xtGlutView::reshape(int w, int h) {}
void xtGlutView::visibility(int visible) {}
void xtGlutView::processmenu(int i) { set_draw_mode(i); }
void xtGlutView::mouseWheel(int button, int dir, int x, int y) {}



//=======================================================================
void xtGlutView::display__(void)
{
	Get_Window()->display();
}

void xtGlutView::idle__(void)
{
	Get_Window()->idle();
}

void xtGlutView::keyboard__(unsigned char key, int x, int y)
{
	Get_Window()->keyboard(key, x, y);
}

void xtGlutView::motion__(int x, int y)
{
	Get_Window()->motion(x, y);
}

void xtGlutView::mouse__(int button, int state, int x, int y)
{
	Get_Window()->mouse(button, state, x, y);
}

void xtGlutView::passivemotion__(int x, int y)
{
	Get_Window()->passivemotion(x, y);
}

void xtGlutView::reshape__(int w, int h)
{
	Get_Window()->reshape(w, h);
}

void xtGlutView::special__(int key, int x, int y)
{
	Get_Window()->keyboard(key, x, y);
}

void xtGlutView::visibility__(int visible)
{
	Get_Window()->visibility(visible);
}

void xtGlutView::processmenu__(int i)
{
	Get_Window()->processmenu(i);
}

void xtGlutView::mouseWheel__(int button, int dir, int x, int y)
{
	Get_Window()->mouseWheel(button, dir, x, y);
}
