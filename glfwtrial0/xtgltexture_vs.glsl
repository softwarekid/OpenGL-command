layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vt;

uniform mat4 xtmatrix; // our matrix

out vec2 texture_coordinates;

void main () {
  
  gl_Position = xtmatrix*vec4 (vertex_position, 1.0);
  //xtcolour = vertex_colour;
  texture_coordinates = vt;
}