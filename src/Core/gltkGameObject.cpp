#include "gltkGameObject.hpp"

#include "gltkGeometryIO.hpp"
#include "gltkRenderManager.hpp"

gltkGameObject::gltkGameObject(std::string name)
{
  Name = name;
}

gltkGameObject::gltkGameObject(std::string name, std::string geometryFile)
{
  Geometry = LoadGeometry(geometryFile);
}

void gltkGameObject::Initialize()
{
  GeometryMapper.FillVBO(Geometry);
}

void gltkGameObject::Render()
{
  GeometryMapper.Render();
}

gltkGeometry gltkGameObject::GetGeometry() const
{
  return Geometry;
}
gltkGeometry& gltkGameObject::GetGeometry()
{
  return Geometry;
}

std::string gltkGameObject::GetName() const
{
  return Name;
}
