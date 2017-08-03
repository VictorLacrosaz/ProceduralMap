#version 120

uniform mat4 camera_projection;
uniform mat4 camera_modelview;

varying vec4 color;

void main (void)
{
  vec4 p = gl_Vertex;
  gl_Position = camera_projection*camera_modelview*p;

  color = gl_Color;
}
