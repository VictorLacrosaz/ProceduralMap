#include "gltkRenderManager.hpp"

#include "glUtils.hpp"
#include "mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>


using namespace cpe;



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
gltkRenderManager::gltkRenderManager()
    :Shaders(0), ProceduralGrid(), Camera()
{}

void gltkRenderManager::Initialize()
{

  // Preload default structure
  Shaders.push_back(read_shader("Default.vert",
                                "Default.frag"));  PRINT_OPENGL_ERROR();
  //Preload grid shader
  Shaders.push_back(ReadShader("Grid.vert",
                               "Grid.frag",{"Position","Normal","Color","T_Coord"}));  PRINT_OPENGL_ERROR();

  // Build ground
  cpe::vec3 PosCam = Camera.GetPosition();
  cpe::vec3 PosGround = PosCam;



  //floor:round down the value
  ProceduralGrid.Build(ProceduralGrid.GetSquareSize()*cpe::vec2(std::floor(PosGround.x()/ProceduralGrid.GetSquareSize()),std::floor(PosGround.z()/ProceduralGrid.GetSquareSize())));
  mesh_ground_opengl.fill_vbo(ProceduralGrid.GetMeshGrid());
}

//---------------------------------------------------------------------------
// Rendering callback
//---------------------------------------------------------------------------
void gltkRenderManager::Render()
{
  // Draw the ground
  SetupShaders();

  cpe::vec3 PosCam = Camera.GetPosition();
  cpe::vec3 PosGround = -PosCam;

  ProceduralGrid.Build(ProceduralGrid.GetSquareSize()*cpe::vec2(std::floor(PosGround.x()/ProceduralGrid.GetSquareSize()),std::floor(PosGround.z()/ProceduralGrid.GetSquareSize())));
  mesh_ground_opengl.fill_vbo(ProceduralGrid.GetMeshGrid());
  mesh_ground_opengl.Render();
}


//---------------------------------------------------------------------------
// Setup default shader for mesh rendering using default texture
//---------------------------------------------------------------------------
void gltkRenderManager::SetupShaders()
{
  for(unsigned int i = 0; i < Shaders.size(); i++)
    {
      //Setup uniform parameters
      glUseProgram(Shaders[i]);                                                   PRINT_OPENGL_ERROR();

      //Set Uniform data to GPU
      glUniformMatrix4fv(get_uni_loc(Shaders[i], "camera_modelview"), 1, false,
                         Camera.GetMatrixModelView().pointer());                  PRINT_OPENGL_ERROR();
      glUniformMatrix4fv(get_uni_loc(Shaders[i], "camera_projection"), 1, false,
                         Camera.GetMatrixProjection().pointer());                 PRINT_OPENGL_ERROR();
      glUniformMatrix4fv(get_uni_loc(Shaders[i], "normal_matrix"), 1, false,
                         Camera.GetMatrixNormal().pointer());                     PRINT_OPENGL_ERROR();

      GLint samplerArrayLoc = glGetUniformLocation(Shaders[i], "texture");
      const GLint samplers[4] = {0,1,2,3};
      glUniform1iv( samplerArrayLoc, 4, samplers );

    }

  for (unsigned int i = 0; i < Textures.size(); i++)
    {
      //load textures
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D,Textures[i]);  PRINT_OPENGL_ERROR();
    }
}

void gltkRenderManager::SetTextures(std::vector<GLuint> t)
{
  Textures = t;
}

