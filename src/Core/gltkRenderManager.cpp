#include "gltkRenderManager.hpp"

#include "glUtils.hpp"
#include "mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>


static cpe::mesh build_ground(float const L,float const h)
{
  cpe::mesh m;
  m.add_vertex(cpe::vec3(-L, h,-L));
  m.add_vertex(cpe::vec3(-L, h, L));
  m.add_vertex(cpe::vec3( L, h, L));
  m.add_vertex(cpe::vec3( L, h,-L));

  m.add_triangle_index({0, 2, 1});
  m.add_triangle_index({0, 3, 2});

  m.fill_color(cpe::vec3(0.8, 0.9, 0.8));

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

  //Preload default structure
  Shaders.push_back(read_shader("Default.vert",
                                "Default.frag"));  PRINT_OPENGL_ERROR();
  //Preload grid shader
  Shaders.push_back(ReadShader("Grid.vert",
                               "Grid.frag",{"Position","Normal","Color","T_Coord"}));  PRINT_OPENGL_ERROR();

  //Build grid
  cpe::vec3 camPos = Camera.GetWorldPosition();
  float tileSize = ProceduralGrid.GetTileSize();
  //floor():round down the value
  cpe::vec2 gridOrigin = tileSize * cpe::vec2(std::floor(camPos.x()/tileSize), std::floor(camPos.z()/tileSize));
  ProceduralGrid.Build(gridOrigin);
  proceduralGridMeshOpenGL.fill_vbo(ProceduralGrid.GetMesh());
}

//---------------------------------------------------------------------------
// Rendering callback
//---------------------------------------------------------------------------
void gltkRenderManager::Render()
{
  SetupShaders();

  //Build grid
  cpe::vec3 camPos = Camera.GetWorldPosition();
  float tileSize = ProceduralGrid.GetTileSize();
  //floor():round down the value
  cpe::vec2 gridOrigin = tileSize * cpe::vec2(std::floor(camPos.x()/tileSize), std::floor(camPos.z()/tileSize));
  ProceduralGrid.Build(gridOrigin);
  proceduralGridMeshOpenGL.fill_vbo(ProceduralGrid.GetMesh());

  //Render map
  proceduralGridMeshOpenGL.Render();
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

