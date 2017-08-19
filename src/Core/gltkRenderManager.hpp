#pragma once

#ifndef GLTK_RENDER_MANAGER_HPP
#define GLTK_RENDER_MANAGER_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "gltkCamera.hpp"
#include "gltkGeometry.hpp"
#include "gltkGeometryMapper.hpp"
#include "gltkProceduralGrid.hpp"

#include <vector>
#include <map>


class gltkRenderManager
{
public:

  gltkRenderManager();

  /**  Method called only once at the beginning (load off files ...) */
  void Initialize();

  /**  Rendering callback */
  void Render();

  /**  Set of available textures */
  void SetTextureBank(std::map<std::string, GLuint> tBank);

  //* Accesors */
  gltkCamera& GetCamera() {return Camera;}
  gltkCamera const& GetCamera() const {return Camera;}

  gltkProceduralGrid& GetGrid() {return ProceduralGrid;}
  gltkProceduralGrid const& GetGrid() const {return ProceduralGrid;}

private:

  /** Setup shader default parameters */
  void SetupShadersDefault();

  void SetupMaterials(gltkGeometry geometry);

  /** Vector of shaders ID */
  std::vector<GLuint> Shaders;

  /** Textures Bank */
  std::map<std::string, GLuint> TextureBank;

  /** Grid mesh for OpenGL drawing */
  gltkGeometryMapper ProceduralGridMeshOpenGL;
  /** Grid */
  gltkProceduralGrid ProceduralGrid;

  /** Main Camera */
  gltkCamera Camera;
};

#endif
