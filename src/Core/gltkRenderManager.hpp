#pragma once

#ifndef GLTK_RENDER_MANAGER_HPP
#define GLTK_RENDER_MANAGER_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "gltkCamera.hpp"
#include "gltkGameObject.hpp"

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

  /** Store game object as a unique pointer*/
  void AddGameObject(gltkGameObject* gameObject);

  /** Get game object raw pointer*/
  gltkGameObject* GetGameObject(std::string name);

  //* Accesors */
  gltkCamera& GetCamera() {return Camera;}
  gltkCamera const& GetCamera() const {return Camera;}


private:

  /** Setup shader default parameters */
  void SetupShadersDefault();

  void SetupMaterials(gltkGeometry const& geometry);

  /** Vector of shaders ID */
  std::vector<GLuint> Shaders;

  /** Textures Bank */
  std::map<std::string, GLuint> TextureBank;

  /** Unique pointers to game objects*/
  std::vector<gltkGameObjectPointer> GameObjects;

  /** Main Camera */
  gltkCamera Camera;
};

#endif
