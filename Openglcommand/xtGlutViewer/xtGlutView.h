#pragma once
#include "gl.hh"
#include <string>
#include <vector>

class xtGlutView
{
public:
	//xtGlutView(void);
	xtGlutView(const char *title, int width, int height);
	virtual ~xtGlutView(void);


protected:

  virtual void draw(const std::string& _drawmode) = 0;
  void clear_draw_modes();
  unsigned int add_draw_mode(const std::string& _s);
  void set_draw_mode(int _id);  



  virtual void display(void);
  virtual void idle(void); 
  virtual void keyboard(int key, int x, int y);
  virtual void motion(int x, int y);
  virtual void mouse(int button, int state, int x, int y);
  virtual void passivemotion(int x, int y);
  virtual void reshape(int w, int h); 
  virtual void visibility(int visible);
  virtual void processmenu(int i);
  virtual void mouseWheel(int button, int dir, int x, int y);

  	int m_Width;
	int m_Height;


private:

  static void display__(void);
  static void idle__(void); 
  static void keyboard__(unsigned char key, int x, int y);
  static void motion__(int x, int y);
  static void mouse__(int button, int state, int x, int y);
  static void passivemotion__(int x, int y);
  static void reshape__(int w, int h); 
  static void special__(int key, int x, int y);   
  static void visibility__(int visible);
  static void processmenu__(int i);
  static void mouseWheel__(int button, int dir, int x, int y);


  static xtGlutView *m_Window;
  static xtGlutView *Get_Window();

private:
	char *m_Title;


	int m_WindowId;

	bool m_Fullscreen;
	int m_BakLeft, m_BakTop, mBakWidth, mBakHeight;

	unsigned int m_DrawMode;
	unsigned int m_NumDrawMode;
	std::vector<std::string> m_DrawModelNames;

	unsigned int m_MenuID;
};

