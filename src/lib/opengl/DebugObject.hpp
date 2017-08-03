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

#ifndef DEBUG_OBJECT_HPP
#define DEBUG_OBJECT_HPP

#include "axes_helper.hpp"
#include "Camera.hpp"
#include "vec3.hpp"

#include "GL/glew.h"
#include "GL/gl.h"

/** Enumeration of DebugObject actions*/
enum DebugAction{DrawLine = 0, DrawAxis, nbOfDebugActions};

/** A container class to draw debug information */
class DebugObject
{
public:
    DebugObject();

    /** Rendering callback */
    void Render(Camera const& camera) const;

    /** Toggle drawing of a line defined by [p1, p2] */
    void DrawLine(cpe::vec3 p1, cpe::vec3 p2);
    void DrawLineOff();

    /** Toggle drawing of the world axis */
    void DrawAxis();
    void DrawAxisOff();


private:

  // Line
  /** Internal line initialization given its extremities */
  void InitializeLine(cpe::vec3 p1, cpe::vec3 p2);
  /** Line rendering callback */
  void RenderLine(Camera const& camera) const;
  /** VBO Line internal storage */
  GLuint vboLine;
  /** Shader ID internal storage */
  GLuint ShaderIDLine;

  // World Axis
  /** Helper class drawing the world axis */
  cpe::axes_helper AxisHelper;
  /** Axis rendering callback */
  void RenderAxis(Camera const& camera) const;

  // Internal storing of actions enable states
  bool ActionState[DebugAction::nbOfDebugActions];
};

#endif
