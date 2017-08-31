#version 400

//Input attributes
in vec3 Position;
in vec3 Normal;
in vec3 Color;
in vec2 TCoord;
in int MaterialID;

//Output attributes
out vec4 v_PositionLC; //Vertex position in local object coordinates
out vec4 v_PositionWC; //Vertex position in world coordinates
out vec3 v_Normal;
out vec4 v_Color;
out vec2 v_TCoord;
flat out int v_MaterialID;

uniform mat4 camera_projection;
uniform mat4 camera_modelview;
uniform mat4 normal_matrix;


void main (void)
{
    gl_Position = camera_projection * camera_modelview * vec4(Position, 1.0f);

    //Compute output attributes
    v_PositionLC = vec4(Position, 1.0f);
    v_PositionWC = camera_modelview * vec4(Position, 1.0f);
    v_Color = vec4(Color, 1.0f);
    vec4 normal4d = normal_matrix * vec4(normalize(Normal), 0.0);
    v_Normal = normal4d.xyz;

    v_TCoord = TCoord;
    v_MaterialID = MaterialID;
}
