
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

#include "gltkRenderInteractor.hpp"

#include "gltkGridTile.hpp"
#include "gltkIntersection.hpp"
#include "glUtils.hpp"
#include "mat3.hpp"
#include "quaternion.hpp"

#include <cmath>
#include <iostream>


gltkRenderInteractor::gltkRenderInteractor()
  :is_left_button(false), is_right_button(false),
  RenderManager()
{
  WindowSize[0] = 800;
  WindowSize[1] = 800;

  EventPosition[0] = 0;
  EventPosition[1] = 0;
  LastEventPosition[0] = 0;
  LastEventPosition[1] = 0;

  MotionFactor = 0.0f;
}

void gltkRenderInteractor::Render()
{
  //Render scene
  RenderManager.Render();

  //Eventually render debug objects
  Debug.Render(RenderManager.GetCamera());
}

void gltkRenderInteractor::Initialize()
{
  //Initialize scene
  RenderManager.Initialize();

  //Setup camera
  float aspect = static_cast<float>(WindowSize[0]) / WindowSize[1];
  RenderManager.GetCamera().SetAspectRatio(aspect);
  cpe::trackball trackBall;
  RenderManager.GetCamera().SetOrientation(trackBall.quat());

  //Setup motion factor
  MotionFactor =
    0.0001f * (1 + 10 * std::abs(RenderManager.GetCamera().GetFocalDistance()));

  //Toggle drawing of world axis
  Debug.DrawAxis();
}

//-------------------------------------------------------
//  Camera transformation
//-------------------------------------------------------
void gltkRenderInteractor::MoveForward()
{
  float const dL =  5.0f * MotionFactor * (EventPosition[1] - LastEventPosition[1]);
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}

void gltkRenderInteractor::MoveZDirection()
{
  float const dL =  5.0f * MotionFactor * (EventPosition[1]-LastEventPosition[1]);
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;

  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}

void gltkRenderInteractor::MoveZDirectionScreen()
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
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const z(0.0f,0.0f,1.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * z));
}


void gltkRenderInteractor::MoveRight()
{
  float const dL = -MotionFactor * (EventPosition[0] - LastEventPosition[0]);
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * x));
}

void gltkRenderInteractor::MoveXDirection()
{
  float const dL = -MotionFactor * (EventPosition[0] - LastEventPosition[0]);
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * x));
}

void gltkRenderInteractor::MoveXDirectionScreen()
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
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const x(-1.0f,0.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  q_CamOrientation.x() = 0;
  q_CamOrientation.z() = 0;
  cam.SetPosition(cameraPosition + dL *(q_CamOrientation * x));
}

void gltkRenderInteractor::MoveUp()
{
  float const dL = MotionFactor * (EventPosition[1]-LastEventPosition[1]);
  gltkCamera& cam = RenderManager.GetCamera();
  cpe::vec3 const y(0.0f,-1.0f,0.0f);
  cpe::vec3 cameraPosition = cam.GetPosition();
  cpe::quaternion q_CamOrientation = conjugated(cam.GetOrientation());
  cam.SetPosition(cameraPosition + dL * (q_CamOrientation * y));
}

void gltkRenderInteractor::TrackBallRotate()
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
    trackBall.quat() = RenderManager.GetCamera().GetOrientation();
    trackBall.set_2d_coords(x0,y0,x1,y1);
    trackBall.apply_rotation();
    RenderManager.GetCamera().SetOrientation(trackBall.quat());
  }
}

void gltkRenderInteractor::TrackBallRotateY()
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

  RenderManager.GetCamera().SetOrientation(RenderManager.GetCamera().GetOrientation()*q);


}

void gltkRenderInteractor::TrackBallZoomWheel()
{
  //magnification factor
  float const F0 = 30.0f;

  float const u = WheelMouv;
  float const fu= u/F0;

  float focalDist = RenderManager.GetCamera().GetFocalDistance();
  focalDist += (std::fabs(focalDist)+1.0f)*fu;
  focalDist = std::min(focalDist,0.0f); //clamp

  //Update motion factor
  MotionFactor = 0.0001f * (1 + 10 * std::abs(focalDist));

  RenderManager.GetCamera().SetFocalDistance(focalDist);

}
void gltkRenderInteractor::TrackBallZoomMouse()
{
  //magnification factor
  float const F0 = 500.0f;

  float const y = EventPosition[1];
  float const y_old = LastEventPosition[1];

  float const u = y-y_old;
  float const fu= u/F0;

  float focalDist = RenderManager.GetCamera().GetFocalDistance();
  focalDist += (std::fabs(focalDist)+1.0f)*fu;
  focalDist = std::min(focalDist,0.0f); //clamp

  //Update motion factor
  MotionFactor = 0.0001f * (1 + 10 * std::abs(focalDist));

  RenderManager.GetCamera().SetFocalDistance(focalDist);
}
//-------------------------------------------------------

//-------------------------------------------------------
//  FPS
//-------------------------------------------------------
void gltkRenderInteractor::increase_frame_number() {++frame;}

unsigned int gltkRenderInteractor::time() const {return current_time;}
unsigned int& gltkRenderInteractor::time() {return current_time;}

float gltkRenderInteractor::update_fps()
{
  current_fps=static_cast<float>(frame*1000.0f)/static_cast<float>(current_time-old_time);
  old_time=current_time;
  frame=0;
  return current_fps;
}

float gltkRenderInteractor::delta_time()
{
  return current_time-old_time;
}

float gltkRenderInteractor::fps()
{
  return current_fps;
}

//-------------------------------------------------------
//  Picking
//-------------------------------------------------------
std::pair<cpe::vec3,cpe::vec3> gltkRenderInteractor::GetPickingRay()
{
  gltkCamera cam = RenderManager.GetCamera();

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

cpe::vec3 gltkRenderInteractor::Pick()
{
//-- Initialization

  //Get pick ray
  std::pair<cpe::vec3,cpe::vec3> rayPair = this->GetPickingRay();
  cpe::vec3 rayStart = rayPair.first;
  cpe::vec3 rayDirection = rayPair.second;

  //Iteratively picked point along the ray
  cpe::vec3 pickPoint = rayStart + rayDirection;

  //Projection plane directions
  cpe::vec2 X(1.0, 0.0);
  cpe::vec2 Z(0.0, 1.0);

  //Find the quadrant in the projection plane XZ corresponding to the ray direction.
  // Quadrant : 00 : up-right
  //            10 : up-left
  //            11 : down-left
  //            01 : down-right
  int quadrant = 0;
  if (rayDirection.x() < 0.0)
  {
    X *= -1.0;
    quadrant += 10;
  }
  if (rayDirection.z() < 0.0)
  {
    Z *= -1.0;
    quadrant += 1;
  }

  //Project ray into the projection plane XZ
  cpe::vec2 rayXZ = normalized(cpe::vec2(rayDirection.x(), rayDirection.z()));
  //Angles between the ray and the plane main directions
  float aX = AngleBetweenVectors(X, rayXZ);
  float aZ = AngleBetweenVectors(Z, rayXZ);

  float tileWidth = RenderManager.GetGrid().GetSquareSize();

  //Current tile origin in world space
  float x_int =tileWidth * floor(pickPoint.x() / tileWidth);
  float z_int =tileWidth * floor(pickPoint.z() / tileWidth);

  //Starting point offset in the tile, relatively to the tile origin.
  float dX = pickPoint.x() - x_int;
  float dZ = pickPoint.z() - z_int;

  //Compute the distance to walk along the ray in order to reach the previous
  //  intersection with a tile axis.
  float dLz = 0.0f;
  float dLx = 0.0f;
  switch (quadrant)
  {
    case 0:
      dLx = dX / fabs(cos(aX*M_PI/180.0));
      dLz = dZ / fabs(cos(aZ*M_PI/180.0));
      break;
    case 10:
      dLx = (tileWidth - dX) / fabs(cos(aX*M_PI/180.0));
      dLz = dZ / fabs(cos(aZ*M_PI/180.0));
      break;
    case 1:
      dLx = dX / fabs(cos(aX*M_PI/180.0));
      dLz = (tileWidth - dZ) / fabs(cos(aZ*M_PI/180.0));
      break;
    case 11:
      dLx = (tileWidth - dX) / fabs(cos(aX*M_PI/180.0));
      dLz = (tileWidth - dZ) / fabs(cos(aZ*M_PI/180.0));
      break;
    default:
      break;
  }

  //Distance to walk along the ray between to successive intersection with
  //  a tile axis.
  float stepX = tileWidth / fabs(cos(aX*M_PI/180.0));
  float stepZ = tileWidth / fabs(cos(aZ*M_PI/180.0));

  //Keep track of the number of intersection we found along each axis.
  unsigned int itStepX = 1;
  unsigned int itStepZ = 1;

//*************************************
  std::vector<cpe::vec3> dbgPts;
//*************************************

//-- Iterations

  while(norm(pickPoint - rayStart) < RenderManager.GetCamera().GetFarDistance())
  {
    //Distance along the projected ray before to the next intersection
    // is reached, for each axis.
    float pointStepX = -dLx + itStepX * stepX;
    float pointStepZ = -dLz + itStepZ * stepZ;

    //Compute distance along the projected ray before to the first intersection
    // and the one just after.
    float pointStep1 = 0.0f;
    float pointStep2 = 0.0f;

    //If the current X intersection is closer than the current Z intersection
    if(pointStepX < pointStepZ)
    {
      //-- 1st intersection
      //Check if the previous Z intersection is closer than our X intersection.
      //If yes, we keep the old Z value for the first intersection, otherwise it's X.
      float previousPointStepZ = -dLz + (itStepZ-1) * stepZ;
      pointStepX > previousPointStepZ ? pointStep1 = pointStepX : pointStep1 = previousPointStepZ;

      //-- 2nd intersection
      //If the next X intersection is closer than the current Z intersection,
      //  choose it for the second intersection.
      if(-dLx + (itStepX+1) * stepX < pointStepZ)
      {
        pointStep2 =-dLx + (itStepX+1) * stepX;
        itStepX++;
      }
      else
      {
        pointStep2 = pointStepZ;
        itStepZ++;
      }
    }
    else
    {
      //-- 1st intersection
      float previousPointStepX = -dLx + (itStepX-1) * stepX;
      pointStepZ > previousPointStepX ? pointStep1 = pointStepZ : pointStep1 = previousPointStepX;

      //-- 2nd intersection
      if(-dLz + (itStepZ+1) * stepZ < pointStepX)
      {
        pointStep2 = -dLz + (itStepZ + 1) * stepZ;
        itStepZ++;
      }
      else
      {
        pointStep2 = pointStepX;
        itStepX++;
      }
    }

    //Distance along the projected ray to the next tile
    float dLxz = (pointStep2 + pointStep1)/2.0f;

    //Project distance to the next tile along the ray
    cpe::vec3 Y(0.0f, 1.0f, 0.0f);
    if (rayDirection.y() < 0.0f)
    {
      Y *= -1.0f;
    }
    float aY = acos(dot(Y,rayDirection));
    //Distance along the ray to the next tile
    float dL = dLxz / sin(aY);

    //*************************************
//    cpe::vec3 pXZ = cpe::vec3(rayStart.x() + rayDirection.x(), 0.0f, rayStart.z() + rayDirection.z());
//    cpe::vec3 p= dLxz * normalized(cpe::vec3(rayXZ.x(), 0.0f, rayXZ.y()));
//    dbgPts.push_back(pXZ + p);//Draw way points XZ
//    cpe::vec3 pXZ = cpe::vec3(pickPoint.x(),0.0f, pickPoint.z());
//    dbgPts.push_back(pickPoint);
    //*************************************


    //Get the tile under the current picked point, before incrementing the ray.
    gltkGridTile currentTile = RenderManager.GetGrid().GetTile(pickPoint.x(),pickPoint.z());

    //Increment picked point along ray
    pickPoint = rayStart + dL * rayDirection;

    //Check intersection with the tile triangles
    // Triangle 1 : 0 2 3 - Triangle 2 : 0 3 1
    const cpe::vec3* tilePoints = currentTile.GetPoints();
    float t1 = gltkIntersection::RayTriangle(rayStart, pickPoint-rayStart,
      tilePoints[0], tilePoints[2], tilePoints[3]);
    float t2 = gltkIntersection::RayTriangle(rayStart, pickPoint-rayStart,
      tilePoints[0], tilePoints[1], tilePoints[3]);

    //If we intersect one of the triangle
    if(t1 > 0.0f || t2 > 0.0f)
    {
      //Get the closest existing intersection
      float t = std::min(t1, t2);
      if(t1 > 0.0f && !(t2 > 0.0f))
      {
        t = t1;
      }
      if(t2 > 0.0f && !(t1 > 0.0f))
      {
        t = t2;
      }

      //Final picked point
      pickPoint = rayStart + t * dL * rayDirection;

      //*************************************
      dbgPts.push_back(pickPoint);
      gltkGridTile pickedTile = RenderManager.GetGrid().GetTile(pickPoint.x(),pickPoint.z());
      dbgPts.push_back(pickedTile.GetPoints()[0]);
      dbgPts.push_back(pickedTile.GetPoints()[1]);
      dbgPts.push_back(pickedTile.GetPoints()[2]);
      dbgPts.push_back(pickedTile.GetPoints()[3]);
      //*************************************

      break;//Intersection found. Stop picking.
    }
  }

  //*************************************
  Debug.DrawPoints(dbgPts);
  Debug.DrawLine(rayStart ,pickPoint);
  //*************************************

  return pickPoint;
}
//-------------------------------------------------------

//-------------------------------------------------------
// Accessors
//-------------------------------------------------------
/** Get/Set render manager */
gltkRenderManager& gltkRenderInteractor::GetRenderManager() {return RenderManager;}
gltkRenderManager const& gltkRenderInteractor::GetRenderManager() const {return RenderManager;}

/** Get/Set window size */
int* gltkRenderInteractor::GetWindowSize() {return WindowSize;}
void gltkRenderInteractor::SetWindowSize( int width, int height )
{
  WindowSize[0] = width;
  WindowSize[1] = height;
  RenderManager.GetCamera().SetAspectRatio(static_cast<float>(width)/height);
}

/** Get/Set event position */
int* gltkRenderInteractor::GetEventPosition() {return EventPosition;}
void gltkRenderInteractor::SetEventPosition(const int x, const int y) {
  EventPosition[0] = x; EventPosition[1] = y;}

/** Get/Set last event position */
int* gltkRenderInteractor::GetLastEventPosition() {return LastEventPosition;}
void gltkRenderInteractor::SetLastEventPosition(const int x, const int y) {
  LastEventPosition[0] = x; LastEventPosition[1] = y;}

bool& gltkRenderInteractor::left_button() {return is_left_button;}
bool gltkRenderInteractor::left_button() const {return is_left_button;}
bool& gltkRenderInteractor::right_button() {return is_right_button;}
bool gltkRenderInteractor::right_button() const {return is_right_button;}
int gltkRenderInteractor::GetWheelMouv() const
{
  return WheelMouv;
}

void gltkRenderInteractor::SetWheelMouv(int value)
{
  WheelMouv = value;
}

