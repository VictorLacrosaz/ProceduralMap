#version 400

//Input attributes
in vec4 v_PositionLC; //Vertex position in local object coordinates
in vec4 v_PositionWC; //Vertex position in world coordinates
in vec3 v_Normal;
in vec4 v_Color;
in vec2 v_TCoord;
flat in int v_MaterialID;

//Output attributes
out vec4 Color;

uniform sampler2D texture[16];

uniform vec3 light = vec3(0.5, 0.3, 5.0);

uniform mat4[32] MaterialsParameters;

//Materials Texture Index
uniform int[32] MaterialsTIdx;

void main (void)
{    
  vec4 Kd = vec4(MaterialsParameters[v_MaterialID][0][0], MaterialsParameters[v_MaterialID][0][1], MaterialsParameters[v_MaterialID][0][2],1.0);
  vec4 Ka = vec4(MaterialsParameters[v_MaterialID][1][0], MaterialsParameters[v_MaterialID][1][1], MaterialsParameters[v_MaterialID][1][2],1.0);
  vec4 Ks = vec4(0.0,0.0,0.0,0.0);

  vec3 n = normalize(v_Normal);

  vec3 p = v_PositionWC.xyz;
  vec3 vertex_to_light = normalize(light - p);
  vec3 reflected_light = reflect(-vertex_to_light, n);
  vec3 user_to_vertex = normalize(-p);

  float diffuse_term = clamp(abs(dot(n, vertex_to_light)), 0.0, 1.0);
  float specular_term = pow(clamp(dot(reflected_light, user_to_vertex), 0.0, 1.0), 128.0);

  vec4 white = vec4(1.0,1.0,1.0,0.0);
  Color = (diffuse_term) * v_Color + specular_term * white;

  if(v_MaterialID > -1 && v_MaterialID < 32)
  {
    vec4 textureColor = vec4(0.0,0.0,0.0,0.0);
    if(MaterialsTIdx[v_MaterialID] > -1)
    {
      textureColor = texture2D(texture[MaterialsTIdx[v_MaterialID]], v_TCoord);
    }

    Color = Ka + diffuse_term * Kd + specular_term * Ks + textureColor;
  }
}
