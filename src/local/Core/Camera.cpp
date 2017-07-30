#include "Camera.hpp"

#include "mat4.hpp"
#include "mat3.hpp"

#include "math.h"

Camera::Camera()
{
  //Projectio parameters
  this->FieldOfView = 55.0f*M_PI/180.0;
  this->NearDistance = 1e-3f;
  this->FarDistance = 500.0f;
  this->AspectRatio = 1.0f;

  //ModelView parameters
  this->Position = cpe::vec3(0.0f,0.0f,0.0f);
  this->Orientation = cpe::quaternion();
  this->FocalDistance = -100.0f;

  //Compute matrices
  this->UpdateMatrixModelView();
  this->UpdateMatrixProjection();
}


//---------------------------------------------------------------------------
// Update ModelView matrix
//---------------------------------------------------------------------------
void Camera::UpdateMatrixModelView()
{
  //Zoom
  cpe::mat4 world_matrix_zoom;
  world_matrix_zoom.set_translation({0.0f,0.0f,this->FocalDistance});

  //Translation
  cpe::mat4 world_matrix_translation;
  world_matrix_translation.set_translation(this->Position);

  //Orientation
  cpe::mat4 world_matrix_rotation = cpe::mat4(this->Orientation.to_mat3());

  //Update ModelView matrix
  this->ModelView = world_matrix_zoom*world_matrix_rotation*world_matrix_translation;

  //Update Normal matrix
  this->Normal = this->BuildNormalMatrix(this->ModelView);
}

//---------------------------------------------------------------------------
// Update Projection matrix
//---------------------------------------------------------------------------
void Camera::UpdateMatrixProjection()
{
  this->Projection.set_projection_perspective(this->FieldOfView,
    this->AspectRatio, this->NearDistance, this->FarDistance);
}

//---------------------------------------------------------------------------
//  Build Normal matrix
//---------------------------------------------------------------------------
cpe::mat4 Camera::BuildNormalMatrix(const cpe::mat4& w)
{
  //the normal matrix is used to compute the transformation of the normals of the meshes
  cpe::mat4 normal_matrix=cpe::mat4(w(0,0),w(1,0),w(2,0),0,
                                    w(0,1),w(1,1),w(2,1),0,
                                    w(0,2),w(1,2),w(2,2),0,
                                    0,0,0,1);
  cpe::vec4 tr(w(0,3),w(1,3),w(2,3),1.0f);
  cpe::vec4 tr_inv=-normal_matrix*tr;
  normal_matrix(0,3)=tr_inv[0];
  normal_matrix(1,3)=tr_inv[1];
  normal_matrix(2,3)=tr_inv[2];

  normal_matrix=transposed(normal_matrix);

  return normal_matrix;
}

//---------------------------------------------------------------------------
//  Projection parameters
//---------------------------------------------------------------------------
std::vector<float> Camera::GetProjectionParameters() const
{
  float const top = tan(FieldOfView * 0.5f) * NearDistance;
  float left= AspectRatio * top;

  std::vector<float> param;
  param.push_back(left);
  param.push_back(top);
  param.push_back(NearDistance);
  param.push_back(FarDistance);
  param.push_back(AspectRatio);

  return param;
}

//---------------------------------------------------------------------------
//  ACCESSORS
//---------------------------------------------------------------------------
void Camera::SetFieldOfView(float fov)
{
  if (fov == this->FieldOfView )
  {
    return;
  }

  this->FieldOfView = fov;
  this->UpdateMatrixProjection();
}

float Camera::GetFieldOfView()
{
  return this->FieldOfView;
}

//---------------------------------------------------------------------------
void Camera::SetNearDistance(float nearDistance)
{
  if (nearDistance == this->NearDistance )
  {
    return;
  }

  this->NearDistance = nearDistance;
  this->UpdateMatrixProjection();
}

float Camera::GetNearDistance()
{
  return this->NearDistance;
}

//---------------------------------------------------------------------------
void Camera::SetFarDistance(float farDistance)
{
  if (farDistance == this->FarDistance )
  {
    return;
  }

  this->FarDistance = farDistance;
  this->UpdateMatrixProjection();
}

float Camera::GetFarDistance()
{
  return this->FarDistance;
}

//---------------------------------------------------------------------------
void Camera::SetAspectRatio(float ratio)
{
  if (ratio == this->AspectRatio )
  {
    return;
  }

  this->AspectRatio = ratio;
  this->UpdateMatrixProjection();
}

float Camera::GetAspectRatio()
{
  return this->AspectRatio;
}

//---------------------------------------------------------------------------
void Camera::SetOrientation(const cpe::quaternion& ori)
{
  this->Orientation = ori;
  this->UpdateMatrixModelView();
}

cpe::quaternion Camera::GetOrientation()
{
  return this->Orientation;
}

//---------------------------------------------------------------------------
void Camera::SetPosition(const cpe::vec3& tr)
{
  if (tr[0] == this->Position[0] &&
    tr[1] == this->Position[1] &&
    tr[2] == this->Position[2])
  {
    return;
  }

  this->Position = tr;
  this->UpdateMatrixModelView();
}

cpe::vec3 Camera::GetPosition()
{
  return this->Position;
}

//---------------------------------------------------------------------------
void Camera::SetFocalDistance(float d)
{
  if (d == this->FocalDistance )
  {
    return;
  }

  this->FocalDistance = d;
  this->UpdateMatrixModelView();
}

float Camera::GetFocalDistance()
{
  return this->FocalDistance;
}

//---------------------------------------------------------------------------
void Camera::SetMatrixModelView(const cpe::mat4& m)
{
  this->ModelView = m;
}

cpe::mat4 Camera::GetMatrixModelView()
{
  return this->ModelView;
}

//---------------------------------------------------------------------------
void Camera::SetMatrixModelProjection(const cpe::mat4& m)
{
  this->Projection = m;
}

cpe::mat4 Camera::GetMatrixProjection()
{
  return this->Projection;
}

//---------------------------------------------------------------------------
cpe::mat4 Camera::GetMatrixNormal()
{
  return this->Normal;
}
