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

#pragma once

#ifndef RENDERINTERACTOR_HPP
#define RENDERINTERACTOR_HPP

#include "DebugObject.hpp"
#include "mat3.hpp"
#include "RenderManager.hpp"
#include "trackball.hpp"
#include "vec3.hpp"

/** \brief
 * Container class for rendering, events and fps manipulation
 */
class RenderInteractor
{
public:

  /** \brief empty constructor */
  RenderInteractor();

  /** \brief Initialisation */
  void Initialize();
  /** \brief Rendering callback */
  void Render();

  //-------------------------------------------------------
  //  Camera transformation
  //-------------------------------------------------------
  /** \brief move the camera forward by a distance dL */
  void MoveForward();
  /** \brief move the camera to the right by a distance dL */
  void MoveRight();
  /** \brief move the camera up by a distance dL */
  void MoveUp();

  /** \brief camera rotation using on-screen coordinates */
  void TrackBallRotate();
  /** \brief camera zoom using on-screen coordinates */
  void TrackBallZoom();

  //-------------------------------------------------------
  //  Picking
  //-------------------------------------------------------
  /** \brief
   * Given (x,y) coordinates in screen space, returns the origin
   * and the direction in world space of the picking ray.
   *
   * \return
   * <origin,direction> : std::pair<cpe::vec3,cpe::vec3>
  */
  std::pair<cpe::vec3,cpe::vec3> GetPickingRay(int x_screen,int y_screen);

  //-------------------------------------------------------
  //  FPS
  //-------------------------------------------------------
  /** \brief increase the frame number by 1 */
  void increase_frame_number();

  /** \brief get the current elapsed time */
  unsigned int time() const;
  /** \brief get the current elapsed time */
  unsigned int& time();

  /** \brief update the fps counter and set frame to 0*/
  float update_fps();
  /** \brief elapsed time since old_time record */
  float delta_time();

  /** \brief get the current fps */
  float fps();

  //-------------------------------------------------------
  //  Accessors
  //-------------------------------------------------------
  /** Get/Set render manager */
  RenderManager& GetRenderManager();
  RenderManager const& GetRenderManager() const;

  /** Get/Set window size */
  int* GetWindowSize();
  void SetWindowSize( int width, int length );

  /** Get/Set event position */
  int* GetEventPosition();
  void SetEventPosition(const int x, const int y);

  /** Get/Set last event position */
  int* GetLastEventPosition();
  void SetLastEventPosition(const int x, const int y);


  /** \brief is left button clicked */
  bool& left_button();
  /** \brief is left button clicked */
  bool left_button() const;

  /** \brief is right button clicked */
  bool& right_button();
  /** \brief is right button clicked */
  bool right_button() const;


private:

  //-------------------------------------------------------
  //  Window and mouse parameters
  //-------------------------------------------------------  
  /** Internal storage of window size */
  int WindowSize[2];

  /** Internal storage of cursor position */
  int EventPosition[2];

  /** Internal storage of previous cursor position */
  int LastEventPosition[2];

  /** \brief internal storage when left button is clicked */
  bool is_left_button;
  /** \brief internal storage when right button is clicked */
  bool is_right_button;

  /** Motion factor used for camera transformation */
  float MotionFactor;

  //-------------------------------------------------------
  //  Time parameter
  //-------------------------------------------------------
  /** \brief internal storage of current fps */
  float current_fps;
  /** \brief internal storage of current time */
  unsigned int current_time;
  /** \brief internal storage of previously recorded time */
  unsigned int old_time;
  /** \brief internal storage of current frame number */
  unsigned int frame;

  //-------------------------------------------------------
  //  Debug helper object
  //-------------------------------------------------------
  /** Helper class for Debug rendering */
  DebugObject Debug;


  /** All the content of the 3D scene */
  RenderManager _RenderManager;
};


#endif
