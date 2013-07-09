#pragma once
#include "xtglutview.h"
#include "Vectors.h"

struct Vec2i
{
	Vec2i() :x(0),y(0) {};
	Vec2i( int _x, int _y ) :x(_x),y(_y){};
	int x, y;
	int operator[](int index) const { return (&x)[index]; };
};



class xtGlutControl :
	public xtGlutView
{
public:
	xtGlutControl(const char *title, int width, int height);
	virtual ~xtGlutControl(void);

  void   set_scene(const Vector3& _center, float _radius);
  void   view_all();
  double measure_fps();

protected:
	virtual void init();
	virtual void draw(const std::string& _draw_mode);
	virtual void reshape(int w, int h); 
	void translate( const Vector3& _trans );
	void update_projection_matrix();
	
	// virtual trackball: map 2D screen point to unit sphere
	bool map_to_sphere(const Vec2i& _point, Vector3& _result);

  // overloaded glut functions
  virtual void motion(int x, int y);
  virtual void mouse(int button, int state, int x, int y);
  //virtual void reshape(int w, int h); 
  virtual void keyboard(int key, int x, int y);

  // mouse processing functions
  void rotation(int x, int y);
  void translation(int x, int y);
  void zoom(int x, int y);
  void rotate( const Vector3& _axis, float _angle );

protected:

  // scene position and dimension
  Vector3    center_;
  float    radius_;


  // projection parameters
  float    near_, far_, fovy_;


  // OpenGL matrices
  double   projection_matrix_[16],
           modelview_matrix_[16];

  
  // trackball helpers
  Vec2i    last_point_2D_;
  Vector3    last_point_3D_;
  bool     last_point_ok_;
  bool     button_down_[10];


};

