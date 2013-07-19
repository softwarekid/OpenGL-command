in vec3 xtcolour;
out vec4 frag_colour;

void main () {
  frag_colour = vec4 (xtcolour, 1.0);
  //frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);
  //gl_FragColor = vec4 ( 0.5, 0.0, 0.5, 1.0 );
}