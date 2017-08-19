#include "gltkMaterial.hpp"

gltkMaterial::gltkMaterial()
{
}

gltkMaterial::gltkMaterial(std::string const& name,
  cpe::vec3 const& ka, cpe::vec3 const& kd,
  std::string KdTexture)
  : Name(name), Ka(ka), Kd(kd), KdTextureFile(KdTexture)
{}

void gltkMaterial::SetName(std::string const& name)
{
  Name = name;
}
std::string gltkMaterial::GetName() const
{
  return Name;
}

void gltkMaterial::SetKa(cpe::vec3 const& ka)
{
  Ka = ka;
}
cpe::vec3 gltkMaterial::GetKa() const
{
  return Ka;
}

void gltkMaterial::SetKd(cpe::vec3 const& kd)
{
  Kd = kd;
}
cpe::vec3 gltkMaterial::GetKd() const
{
  return Kd;
}

void gltkMaterial::SetKdTexture(std::string filename)
{
  KdTextureFile = filename;
}
std::string gltkMaterial::GetKdTexture()
{
  return KdTextureFile;
}

