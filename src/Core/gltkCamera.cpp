#include "gltkCamera.hpp"

#include "mat4.hpp"
#include "mat3.hpp"

#include "math.h"

gltkCamera::gltkCamera()
{
  //Projection parameters
  this->FieldOfView = 55.0f*M_PI/180.0;
  this->NearDistance = 1e0f;
  this->FarDistance = 10000.0f;
  this->AspectRatio = 1.0f;

  //ModelView parameters
  this->Position = cpe::vec3(0.0f,-200.0f,0.0f);
  this->Orientation = cpe::quaternion(0.0f,1.0f,0.0f,1.0f);
  this->FocalDistance = -1200.0f;

  //Compute matrices
  this->UpdateMatrixModelView();
  this->UpdateMatrixProjection();
}


//---------------------------------------------------------------------------
// Update ModelView matrix
//---------------------------------------------------------------------------
void gltkCamera::UpdateMatrixModelView()
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
void gltkCamera::UpdateMatrixProjection()
{
  this->Projection.set_projection_perspective(this->FieldOfView,
                                              this->AspectRatio, this->NearDistance, this->FarDistance);
}

//---------------------------------------------------------------------------
//  Build Normal matrix
//---------------------------------------------------------------------------
cpe::mat4 gltkCamera::BuildNormalMatrix(const cpe::mat4& w)
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
std::vector<float> gltkCamera::GetProjectionParameters() const
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
void gltkCamera::SetFieldOfView(float fov)
{
  if (fov == this->FieldOfView )
  {
    return;
  }

  this->FieldOfView = fov;
  this->UpdateMatrixProjection();
}

float gltkCamera::GetFieldOfView()
{
  return this->FieldOfView;
}

//---------------------------------------------------------------------------
void gltkCamera::SetNearDistance(float nearDistance)
{
  if (nearDistance == this->NearDistance )
  {
    return;
  }

  this->NearDistance = nearDistance;
  this->UpdateMatrixProjection();
}

float gltkCamera::GetNearDistance()
{
  return this->NearDistance;
}

//---------------------------------------------------------------------------
void gltkCamera::SetFarDistance(float farDistance)
{
  if (farDistance == this->FarDistance )
  {
    return;
  }

  this->FarDistance = farDistance;
  this->UpdateMatrixProjection();
}

float gltkCamera::GetFarDistance()
{
  return this->FarDistance;
}

//---------------------------------------------------------------------------
void gltkCamera::SetAspectRatio(float ratio)
{
  if (ratio == this->AspectRatio )
  {
    return;
  }

  this->AspectRatio = ratio;
  this->UpdateMatrixProjection();
}

float gltkCamera::GetAspectRatio() const
{
  return this->AspectRatio;
}

//---------------------------------------------------------------------------
void gltkCamera::SetOrientation(const cpe::quaternion& ori)
{
  this->Orientation = ori;
  this->UpdateMatrixModelView();
}

cpe::quaternion gltkCamera::GetOrientation() const
{
  return this->Orientation;
}

//---------------------------------------------------------------------------
void gltkCamera::SetPosition(const cpe::vec3& tr)
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

cpe::vec3 gltkCamera::GetPosition()
{
  return this->Position;
}

cpe::vec3 gltkCamera::GetWorldPosition()
{

  cpe::vec3 p = conjugated(this->Orientation) *
      cpe::vec3(0.0f,0.0f,this->FocalDistance);

  p += this->Position;
  return -p;
}

//---------------------------------------------------------------------------
void gltkCamera::SetFocalDistance(float d)
{
  if (d == this->FocalDistance )
  {
    return;
  }

  this->FocalDistance = d;
  this->UpdateMatrixModelView();
}

float gltkCamera::GetFocalDistance()
{
  return this->FocalDistance;
}

//---------------------------------------------------------------------------
void gltkCamera::SetMatrixModelView(const cpe::mat4& m)
{
  this->ModelView = m;
}

cpe::mat4 gltkCamera::GetMatrixModelView() const
{
  return this->ModelView;
}


//---------------------------------------------------------------------------
void gltkCamera::SetMatrixModelProjection(const cpe::mat4& m)
{
  this->Projection = m;
}

cpe::mat4 gltkCamera::GetMatrixProjection() const
{
  return this->Projection;
}

//---------------------------------------------------------------------------
cpe::mat4 gltkCamera::GetMatrixNormal()
{
  return this->Normal;
}
