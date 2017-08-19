#include "gltkRenderManager.hpp"

#include "glUtils.hpp"
#include "gltkGeometryIO.hpp"

#include <cmath>
#include <string>
#include <sstream>


static gltkGeometry build_ground(float const L,float const h)
{
  gltkGeometry m;
  m.AddVertex(cpe::vec3(-L, h,-L));
  m.AddVertex(cpe::vec3(-L, h, L));
  m.AddVertex(cpe::vec3( L, h, L));
  m.AddVertex(cpe::vec3( L, h,-L));

  m.AddTriangleIndex(cpe::triangle_index(0, 2, 1));
  m.AddTriangleIndex(cpe::triangle_index(0, 3, 2));

  m.FillColorXYZ();

  m.FillEmptyFields();

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
  Shaders.push_back(ReadShader("DefaultMultiMaterial.vert",
                               "DefaultMultiMaterial.frag",
  {"Position", "Normal", "Color", "T_Coord"}));  PRINT_OPENGL_ERROR();

  //Build grid
  cpe::vec3 camPos = -1.0 * Camera.GetPosition();
  float tileSize = ProceduralGrid.GetTileSize();
  //floor():round down the value
  cpe::vec2 gridOrigin = tileSize * cpe::vec2(std::floor(camPos.x()/tileSize), std::floor(camPos.z()/tileSize));
  ProceduralGrid.Build(gridOrigin);
  ProceduralGridMeshOpenGL.FillVBO(ProceduralGrid.GetGeometry());
}

//---------------------------------------------------------------------------
// Rendering callback
//---------------------------------------------------------------------------
void gltkRenderManager::Render()
{
  SetupShadersDefault();

  //Build grid
  cpe::vec3 camPos = -1.0 * Camera.GetPosition();
  float tileSize = ProceduralGrid.GetTileSize();
  //floor():round down the value
  cpe::vec2 gridOrigin = tileSize * cpe::vec2(std::floor(camPos.x()/tileSize), std::floor(camPos.z()/tileSize));
  ProceduralGrid.Build(gridOrigin);
  ProceduralGridMeshOpenGL.FillVBO(ProceduralGrid.GetGeometry());

  //Render map
  SetupMaterials(ProceduralGrid.GetGeometry());
  ProceduralGridMeshOpenGL.Render();

  SetupMaterials(objMesh);
  objMeshOpengl.Render();
}


//---------------------------------------------------------------------------
// Setup shader default parameters
//---------------------------------------------------------------------------
void gltkRenderManager::SetupShadersDefault()
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
    const GLint samplers[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    glUniform1iv( samplerArrayLoc, 16, samplers );
  }
}

//---------------------------------------------------------------------------
// Setup shader materials parameters
//---------------------------------------------------------------------------
void gltkRenderManager::SetupMaterials(gltkGeometry geometry)
{
  //Store material uniforms in mat4
  std::vector<cpe::mat4> MaterialsUniforms;
  //Store material texture index in sampler
  std::vector<int> MaterialsTIdx;
  //Keep track of the number of textured materials
  int nbTexturedMaterials = 0;

  for(int i = 0; i < geometry.GetNumberOfMaterials(); i++)
  {
    // Load uniform shading parameters
    cpe::mat4 materialUniforms;
    for(int k = 0; k < 3; k++)
      materialUniforms(k,0) = geometry.GetMaterials()[i].GetKd()[k];
    for(int k = 0; k < 3; k++)
      materialUniforms(k,1) = geometry.GetMaterials()[i].GetKa()[k];
    //Add uniforms
    MaterialsUniforms.push_back(materialUniforms);

    // Load textures
    int textureIdx = -1;//Texture index in sampler. (-1) = No texture
    std::string textureFilename = geometry.GetMaterials()[i].GetKdTexture();
    if(textureFilename != "")
    {
      //Find texture in texture bank
      if(TextureBank.find(textureFilename) != TextureBank.end())
      {
        GLuint textureID = TextureBank[textureFilename];
        glActiveTexture(GL_TEXTURE0 + nbTexturedMaterials);
        glBindTexture(GL_TEXTURE_2D, textureID);  PRINT_OPENGL_ERROR();

        textureIdx = nbTexturedMaterials;
        nbTexturedMaterials ++;
      }
      else
      {
        std::cout<<"Material texture not found in texture bank : "
                <<textureFilename<<std::endl;
      }
    }
    //Add texture index or -1
    MaterialsTIdx.push_back(textureIdx);
  }

  //Upload materials uniforms
  glUniformMatrix4fv(get_uni_loc(Shaders[1], "MaterialsParameters"), geometry.GetMaterials().size(), false,
      MaterialsUniforms[0].pointer());
  //Upload materials texture indexes
  glUniform1iv(get_uni_loc(Shaders[1], "MaterialsTIdx"), geometry.GetMaterials().size(),
      &MaterialsTIdx[0]);
}

void gltkRenderManager::SetTextureBank(std::map<std::string, GLuint> tBank)
{
  TextureBank = tBank;
}
