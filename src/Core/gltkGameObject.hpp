#pragma once

#ifndef GLTK_GAME_OBJECT_HPP
#define GLTK_GAME_OBJECT_HPP

#include "gltkGeometry.hpp"
#include "gltkGeometryMapper.hpp"

#include <memory>
#include <string>

class gltkRenderManager; //Forward declaration workaround for circular dependency

class gltkGameObject
{
public:

  /** \brief Constructor assigning a name to the game object */
  gltkGameObject(std::string name);

  /** \brief Construct GameObject from a geometry file */
  gltkGameObject(std::string name, std::string geometryFile);

  virtual void Initialize();
  virtual void Render();

  /** \brief Update callback called before rendering */
  virtual void Update(gltkRenderManager const& rm){}

  gltkGeometry GetGeometry() const;
  gltkGeometry& GetGeometry();

  std::string GetName() const;

protected:
  //Geometry
  gltkGeometry Geometry;
  gltkGeometryMapper GeometryMapper;

private:
  std::string Name;

};

typedef std::unique_ptr<gltkGameObject> gltkGameObjectPointer;

#endif // GLTK_GAME_OBJECT_HPP
