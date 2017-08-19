#pragma once

#ifndef GLTK_MATERIAL_HPP
#define GLTK_MATERIAL_HPP

#include "vec3.hpp"

#include "GL/glew.h"
#include <string>



class gltkMaterial
{
public:

  gltkMaterial();

  gltkMaterial(std::string const& name, cpe::vec3 const& ka, cpe::vec3 const& kd, std::string KdTexture);

  void SetName(std::string const& name);
  std::string GetName() const;

  void SetKa(cpe::vec3 const& ka);
  cpe::vec3 GetKa() const;

  void SetKd(cpe::vec3 const& kd);
  cpe::vec3 GetKd() const;

  void SetKdTexture(std::string filename);
  std::string GetKdTexture();

private:
  std::string Name;

  cpe::vec3 Ka;
  cpe::vec3 Kd;

  std::string KdTextureFile;

};

#endif
