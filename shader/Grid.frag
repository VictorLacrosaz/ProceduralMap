#version 130

//Input attributes
in vec4 v_PositionLC; //Vertex position in local object coordinates
in vec4 v_PositionWC; //Vertex position in world coordinates
in vec3 v_Normal;
flat in vec4 v_Color;
in vec2 v_TCoord;

//Output attributes
out vec4 Color;

uniform sampler2D texture[4];

uniform vec3 light = vec3(0.5, 0.3, 5.0);


void main (void)
{
  vec3 n = normalize(v_Normal);

  vec3 p = v_PositionWC.xyz;
  vec3 vertex_to_light = normalize(light - p);
  vec3 reflected_light = reflect(-vertex_to_light, n);
  vec3 user_to_vertex = normalize(-p);

  float diffuse_term = 0.8 * clamp(abs(dot(n, vertex_to_light)), 0.0, 1.0);
  float specular_term = 0.2 * pow(clamp(dot(reflected_light, user_to_vertex), 0.0, 1.0), 128.0);
  float ambiant_term = 0.4;

  vec4 white = vec4(1.0,1.0,1.0,0.0);
  vec4 color_final=vec4(1,0,0,0);

  vec4 ColorTexture0;
  vec4 ColorTexture1;
  vec4 ColorTexture2;
  vec4 ColorTexture3;


  ColorTexture0 = texture2D(texture[0],v_TCoord);
  ColorTexture1 = texture2D(texture[1],v_TCoord);
  ColorTexture2 = texture2D(texture[2],v_TCoord);
  ColorTexture3 = texture2D(texture[3],v_TCoord);

  color_final = ColorTexture1;

  if (v_Color.r == 0.25)
  {
    color_final = ColorTexture3;
  }
  if (v_Color.r == 0.5)
  {
    color_final = ColorTexture0;
  }
  if (v_Color.r == 0.75)
  {
    color_final = ColorTexture2;
  }

  Color = (ambiant_term + diffuse_term) * color_final + specular_term * white;
}
