#include "RenderManager.hpp"

#include "glutils.hpp"
#include "mesh_io.hpp"
#include "perlin.hpp"

#include <cmath>
#include <string>
#include <sstream>


using namespace cpe;

static cpe::mesh BuildGrid (vec2 Center,int Radius)
{
  mesh m;
  int u = 0,v = 0;
  int Size  = 2*Radius+1;
  for (int i = Center.x()-Radius;i <= Center.x()+Radius;i++)
  {
    for (int j = Center.y()-Radius;j <= Center.y()+Radius;j++)
    {
        m.add_vertex(vec3(i,j,0));
        if(std::pow(i,2)+std::pow(j,2) < std::pow(Radius,2))
        {
            if(u < Size-1 && v < Size-1)
            {

                m.add_triangle_index({u+v*Size,u+(v+1)*Size,(u+1)+v*Size});
                m.add_triangle_index({u+(v+1)*Size,u+1+(v+1)*Size,(u+1)+v*Size});
            }
        }
        u++;
    }

    u = 0;
    v++;
  }

  m.fill_color(vec3(0.8,0.6,0.6));

 return m;
}


static cpe::mesh build_ground(float const L,float const h)
{
    mesh m;
    m.add_vertex(vec3(-L, h,-L));
    m.add_vertex(vec3(-L, h, L));
    m.add_vertex(vec3( L, h, L));
    m.add_vertex(vec3( L, h,-L));

    m.add_triangle_index({0, 2, 1});
    m.add_triangle_index({0, 3, 2});

    m.fill_color(vec3(0.8, 0.9, 0.8));

    return m;
}

//---------------------------------------------------------------------------
// Constructors
//---------------------------------------------------------------------------
RenderManager::RenderManager()
    :ShaderDefault(0), MainCamera()
{}

void RenderManager::Initialize()
{
  // Preload default structure
  ShaderDefault = read_shader("Default.vert",
                              "Default.frag");  PRINT_OPENGL_ERROR();

  // Build ground
  mesh_ground = BuildGrid(vec2(0, 0) ,10);
  mesh_ground.fill_empty_field_by_default();
  mesh_ground_opengl.fill_vbo(mesh_ground);
}

//---------------------------------------------------------------------------
// Rendering callback
//---------------------------------------------------------------------------
void RenderManager::Render()
{
  // Draw the ground
  SetupShaderDefault();
  mesh_ground_opengl.draw();
}


//---------------------------------------------------------------------------
// Setup default shader for mesh rendering using default texture
//---------------------------------------------------------------------------
void RenderManager::SetupShaderDefault()
{
  //Setup uniform parameters
  glUseProgram(ShaderDefault);                                               PRINT_OPENGL_ERROR();

  //Set Uniform data to GPU
  glUniformMatrix4fv(get_uni_loc(ShaderDefault, "camera_modelview"), 1, false,
    MainCamera.GetMatrixModelView().pointer());                              PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(ShaderDefault, "camera_projection"), 1, false,
    MainCamera.GetMatrixProjection().pointer());                             PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(ShaderDefault, "normal_matrix"), 1, false,
    MainCamera.GetMatrixNormal().pointer());                                 PRINT_OPENGL_ERROR();

  //load white texture
  glBindTexture(GL_TEXTURE_2D, TextureDefault);                               PRINT_OPENGL_ERROR();
}


void RenderManager::SetTextureDefault(GLuint t)
{
  TextureDefault = t;
}

