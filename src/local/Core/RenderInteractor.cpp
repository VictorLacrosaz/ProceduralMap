
/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "RenderInteractor.hpp"

#include "glutils.hpp"
#include "mat3.hpp"
#include "quaternion.hpp"

#include <cmath>
#include <iostream>


RenderInteractor::RenderInteractor()
  :is_left_button(false), is_right_button(false),
  _RenderManager()
{
  WindowSize[0] = 800;
  WindowSize[1] = 800;

  EventPosition[0] = 0;
  EventPosition[1] = 0;
  LastEventPosition[0] = 0;
  LastEventPosition[1] = 0;

  MotionFactor = 0.0f;
}

void RenderInteractor::Render()
{
  //Render scene
  _RenderManager.Render();

  //Eventually render debug objects
  Debug.Render(_RenderManager.GetCamera());
}

void RenderInteractor::Initialize()
{
  //Initialize scene
  _RenderManager.Initialize();

  //Setup camera
  float aspect = static_cast<float>(WindowSize[0]) / WindowSize[1];
  _RenderManager.GetCamera().SetAspectRatio(aspect);
  cpe::trackball trackBall;
  _RenderManager.GetCamera().SetOrientation(trackBall.quat());

  //Setup motion factor
  MotionFactor =
    0.0001f * (1 + 10 * std::abs(_RenderManager.GetCamera().GetFocalDistance()));

  //Toggle drawing of world axis
  Debug.DrawAxis();
}

//-------------------------------------------------------
//  Camera transformation
//-------------------------------------------------------
void RenderInteractor::MoveForward()
{
  float const dL =  5.0f * MotionFactor * (EventPosition[1]-LastEventPosition[1]);
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}

void RenderInteractor::MoveZDirection()
{
  float const dL =  5.0f * MotionFactor * (EventPosition[1]-LastEventPosition[1]);
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;

  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}

void RenderInteractor::MoveZDirectionScreen()
{
  float dL;
  if(EventPosition[1] < 0.05*WindowSize[1])
    {
      dL = MotionFactor * 10;
    }
  else
    {
      dL = -MotionFactor * 10;
    }
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}


void RenderInteractor::MoveRight()
{
  float const dL = -MotionFactor * (EventPosition[0] - LastEventPosition[0]);
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * x));
}

void RenderInteractor::MoveXDirection()
{
  float const dL = -MotionFactor * (EventPosition[0] - LastEventPosition[0]);
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * x));
}

void RenderInteractor::MoveXDirectionScreen()
{
    float dL;
  if(EventPosition[0] < 0.05*WindowSize[0])
    {
      dL = -MotionFactor * 10;
    }
  else
    {
      dL = MotionFactor * 10;
    }
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL *(q_CamOrientation * x));
}

void RenderInteractor::MoveUp()
{
  float const dL = MotionFactor * (EventPosition[1]-LastEventPosition[1]);
  Camera& cam = _RenderManager.GetCamera();
  cpe::vec3 const y(0.0f,-1.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * y));
}

void RenderInteractor::TrackBallRotate()
{
  int const x = EventPosition[0];
  int const y = EventPosition[1];

  float const x_old = LastEventPosition[0];
  float const y_old = LastEventPosition[1];
  float const h = WindowSize[1];
  float const w = WindowSize[0];

  float const x0 = (2.0f * x_old - w) / w;
  float const y0 = (h - 2.0f * y_old) / h;
  float const x1 = (2.0f * x - w) / w;
  float const y1 = (h - 2.0f * y) / h;
  if (std::sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1)) > 1e-4)
  {
    cpe::trackball trackBall;
    trackBall.quat() = _RenderManager.GetCamera().GetOrientation();
    trackBall.set_2d_coords(x0,y0,x1,y1);
    trackBall.apply_rotation();
    _RenderManager.GetCamera().SetOrientation(trackBall.quat());
  }
}

void RenderInteractor::TrackBallRotateY()
{
  cpe::vec2 Pos = cpe::vec2(EventPosition[0], EventPosition[1]);
  cpe::vec2 LastPos = cpe::vec2(LastEventPosition[0], LastEventPosition[1]);

  float const h = WindowSize[1];
  float const w = WindowSize[0];
  Pos = Pos - cpe::vec2(w/2,h/2);
  LastPos = LastPos - cpe::vec2(w/2,h/2);

  float sign = Pos.x() * LastPos.y() - Pos.y() * LastPos.x();

  sign > 0.0f ? sign = 1.0f : sign=-1.0f;

  float diff = sign * cpe::norm(Pos-LastPos)/w;
  cpe::quaternion q;
  q.set_axis_angle(cpe::vec3(0.0f,1.0f,0.0f),diff);

  _RenderManager.GetCamera().SetOrientation(_RenderManager.GetCamera().GetOrientation()*q);


}

void RenderInteractor::TrackBallZoomWheel()
{
  //magnification factor
  float const F0 = 30.0f;

  float const u = WheelMouv;
  float const fu= u/F0;

  float focalDist = _RenderManager.GetCamera().GetFocalDistance();
  focalDist += (std::fabs(focalDist)+1.0f)*fu;
  focalDist = std::min(focalDist,0.0f); //clamp

  //Update motion factor
  MotionFactor = 0.0001f * (1 + 10 * std::abs(focalDist));

  _RenderManager.GetCamera().SetFocalDistance(focalDist);

}
void RenderInteractor::TrackBallZoomMouse()
{
  //magnification factor
  float const F0 = 500.0f;

  float const y = EventPosition[1];
  float const y_old = LastEventPosition[1];

  float const u = y-y_old;
  float const fu= u/F0;

  float focalDist = _RenderManager.GetCamera().GetFocalDistance();
  focalDist += (std::fabs(focalDist)+1.0f)*fu;
  focalDist = std::min(focalDist,0.0f); //clamp

  //Update motion factor
  MotionFactor = 0.0001f * (1 + 10 * std::abs(focalDist));

  _RenderManager.GetCamera().SetFocalDistance(focalDist);
}
//-------------------------------------------------------

//-------------------------------------------------------
//  FPS
//-------------------------------------------------------
void RenderInteractor::increase_frame_number() {++frame;}

unsigned int RenderInteractor::time() const {return current_time;}
unsigned int& RenderInteractor::time() {return current_time;}

float RenderInteractor::update_fps()
{
  current_fps=static_cast<float>(frame*1000.0f)/static_cast<float>(current_time-old_time);
  old_time=current_time;
  frame=0;
  return current_fps;
}

float RenderInteractor::delta_time()
{
  return current_time-old_time;
}

float RenderInteractor::fps()
{
  return current_fps;
}

//-------------------------------------------------------
//  Picking
//-------------------------------------------------------
std::pair<cpe::vec3,cpe::vec3> RenderInteractor::GetPickingRay()
{
  Camera cam = _RenderManager.GetCamera();

  //Normalized screen coordinates (x[-1;1], y[1;-1])
  float const local_x = (WindowSize[0] - 2.0f * EventPosition[0]) /
    static_cast<float>(WindowSize[0]);
  float const local_y = (2.0f * EventPosition[1] - WindowSize[1]) /
    static_cast<float>(WindowSize[1]);

  //Camera projection parameters
  std::vector<float> const cameraParam = cam.GetProjectionParameters();
  float const Lh=-cameraParam[1];
  float const aspect=cameraParam[4];
  float const znear = cameraParam[2];

  //Normalized projected coordinates
  cpe::vec3 const rayProjection(local_x * Lh * aspect, local_y * Lh, -znear);

  //Unproject ray
  cpe::mat3 const modelViewInv = transposed(cam.GetOrientation().to_mat3());
  cpe::vec3 const rayDirection = normalized(modelViewInv * rayProjection);
  //Compute ray origin
  cpe::vec3 cameraZoom(0.0f,0.0f,-cam.GetFocalDistance());
  cpe::vec3 const rayStart = -cam.GetPosition() + (modelViewInv * cameraZoom);

  //Return ray origin and direction in world coordinates
  return std::make_pair(rayStart,rayDirection);
}

cpe::vec3 RenderInteractor::Pick()
{
  //Get pick ray
  std::pair<cpe::vec3,cpe::vec3> rayPair = this->GetPickingRay();
  cpe::vec3 rayStart = rayPair.first;
  cpe::vec3 rayDirection = rayPair.second;

  cpe::vec3 pickPoint = rayStart + rayDirection;

  //-- WIP : Pick along ray with steps of 10.0f and draw line for debug purpose
  int i = 2;
  while(i< 100)
  {
    pickPoint = rayStart+ i * 10.0f * rayDirection;
    i++;
    if(pickPoint.y()<0.0)
    {
      break;
    }
  }
  Debug.DrawLine(rayStart ,pickPoint);
  // -- end WIP

  return pickPoint;
}
//-------------------------------------------------------

//-------------------------------------------------------
// Accessors
//-------------------------------------------------------
/** Get/Set render manager */
RenderManager& RenderInteractor::GetRenderManager() {return _RenderManager;}
RenderManager const& RenderInteractor::GetRenderManager() const {return _RenderManager;}

/** Get/Set window size */
int* RenderInteractor::GetWindowSize() {return WindowSize;}
void RenderInteractor::SetWindowSize( int width, int height )
{
  WindowSize[0] = width;
  WindowSize[1] = height;
  _RenderManager.GetCamera().SetAspectRatio(static_cast<float>(width)/height);
}

/** Get/Set event position */
int* RenderInteractor::GetEventPosition() {return EventPosition;}
void RenderInteractor::SetEventPosition(const int x, const int y) {
  EventPosition[0] = x; EventPosition[1] = y;}

/** Get/Set last event position */
int* RenderInteractor::GetLastEventPosition() {return LastEventPosition;}
void RenderInteractor::SetLastEventPosition(const int x, const int y) {
  LastEventPosition[0] = x; LastEventPosition[1] = y;}

bool& RenderInteractor::left_button() {return is_left_button;}
bool RenderInteractor::left_button() const {return is_left_button;}
bool& RenderInteractor::right_button() {return is_right_button;}
bool RenderInteractor::right_button() const {return is_right_button;}
int RenderInteractor::GetWheelMouv() const
{
  return WheelMouv;
}

void RenderInteractor::SetWheelMouv(int value)
{
  WheelMouv = value;
}

