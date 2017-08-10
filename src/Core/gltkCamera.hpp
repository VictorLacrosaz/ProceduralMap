#pragma once

#ifndef GLTK_CAMERA_HPP
#define GLTK_CAMERA_HPP

#include "mat4.hpp"
#include "quaternion.hpp"
#include "vec3.hpp"

#include <vector>


class gltkCamera
{
public:

  /** \brief empty constructor */
  gltkCamera();

  void UpdateMatrixModelView();
  void UpdateMatrixProjection();

  std::vector<float> GetProjectionParameters() const;

  void SetFieldOfView(float fov);
  float GetFieldOfView();
  void SetNearDistance(float nearDistance);
  float GetNearDistance();
  void SetFarDistance(float farDistance);
  float GetFarDistance();
  void SetAspectRatio(float ratio);
  float GetAspectRatio() const;

  void SetOrientation(const cpe::quaternion& ori);
  cpe::quaternion GetOrientation() const;
  void SetPosition(const cpe::vec3& tr);
  /** Get the absolute Camera position */
  cpe::vec3 GetPosition();
  /** Get Camera Wordl position including zoom*/
  cpe::vec3 GetWorldPosition();
  void SetFocalDistance(float d);
  float GetFocalDistance();

  void SetMatrixModelView(const cpe::mat4& m);
  cpe::mat4 GetMatrixModelView() const;
  void SetMatrixModelProjection(const cpe::mat4& m);
  cpe::mat4 GetMatrixProjection() const;
  cpe::mat4 GetMatrixNormal();


protected:

  cpe::mat4 BuildNormalMatrix( const cpe::mat4& m);

  //Projection parameters
  /** \brief internal camera FOV storage */
  float FieldOfView;
  /** \brief internal camera near clipping distance */
  float NearDistance;
  /** \brief internal camera far clipping distance */
  float FarDistance;
  /** \brief internal camera aspect ratio */
  float AspectRatio;

  //ModelView parameters
  /** \brief internal camera orientation quaternion */
  cpe::quaternion Orientation;
  /** \brief internal camera world space position */
  cpe::vec3 Position;
  /** \brief internal zoom magnitude (z-translation in local coordinate frame) */
  float FocalDistance;

  //Camera matrices
  cpe::mat4 ModelView;
  cpe::mat4 Projection;
  cpe::mat4 Normal;

};

#endif // CAMERA_HPP
