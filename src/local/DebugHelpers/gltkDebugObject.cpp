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

#include "gltkDebugObject.hpp"
#include "error_handling.hpp"
#include "glutils.hpp"
#include "mat3.hpp"


gltkDebugObject::gltkDebugObject()
    :vboLine(0),ShaderIDPrimitives(0)
{
  for (int i = gltkDebugAction::DrawAxis; i < gltkDebugAction::nbOfDebugActions; i++)
  {
    ActionState[i] = false;
  }
}

//-------------------------------------------------------
// Objects rendering callback
//-------------------------------------------------------
void gltkDebugObject::Render(gltkCamera const& camera) const
{
  if (ActionState[gltkDebugAction::DrawPoints])
  {
    this->RenderPoints(camera);
  }

  if (ActionState[gltkDebugAction::DrawLine])
  {
    this->RenderLine(camera);
  }

  if (ActionState[gltkDebugAction::DrawAxis])
  {
    this->RenderAxis(camera);
  }
}

//-------------------------------------------------------
// Toggle drawing of a set of point
//-------------------------------------------------------
void gltkDebugObject::DrawPoints(std::vector<cpe::vec3> pts)
{
  ActionState[gltkDebugAction::DrawPoints] = true;
  InitializePoints(pts);
}

void gltkDebugObject::DrawPointsOff()
{
  ActionState[gltkDebugAction::DrawPoints] = false;
}
//-------------------------------------------------------


//-------------------------------------------------------
// Toggle drawing of a line defined by [p1, p2]
//-------------------------------------------------------
void gltkDebugObject::DrawLine(cpe::vec3 p1, cpe::vec3 p2)
{
  ActionState[gltkDebugAction::DrawLine] = true;
  InitializeLine(p1, p2);
}

void gltkDebugObject::DrawLineOff()
{
  ActionState[gltkDebugAction::DrawLine] = false;
}
//-------------------------------------------------------

//-------------------------------------------------------
// Toggle drawing of the world axis
//-------------------------------------------------------
void gltkDebugObject::DrawAxis()
{
  ActionState[gltkDebugAction::DrawAxis] = true;
  AxisHelper.init();
}

void gltkDebugObject::DrawAxisOff()
{
  ActionState[gltkDebugAction::DrawAxis] = false;
}
//-------------------------------------------------------

//-------------------------------------------------------
// Internal initialization of points
//-------------------------------------------------------
void gltkDebugObject::InitializePoints(std::vector<cpe::vec3> pts)
{
  this->NbPoints = pts.size();

  if(NbPoints == 0)
  {
    return;
  }

  //load shaders
  ShaderIDPrimitives = read_shader("DebugPrimitives.vert",
                             "DebugPrimitives.frag");

  if(vboPoints == 0)
  {
    glGenBuffers(1, &vboPoints); PRINT_OPENGL_ERROR();
    ASSERT_CPE(vboPoints != 0, "Problem initializing VBO");
  }
  //Send data on the GPU
  glBindBuffer(GL_ARRAY_BUFFER, vboPoints);                                         PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, pts.size()*3*sizeof(float), pts[0].pointer(), GL_DYNAMIC_DRAW);  PRINT_OPENGL_ERROR();
}

//-------------------------------------------------------
// Points Rendering Callback
//-------------------------------------------------------
void gltkDebugObject::RenderPoints(gltkCamera const& camera) const
{
  if(!vboPoints || NbPoints == 0)
  {
    return;
  }
  ASSERT_CPE(vboPoints != 0,"VBO for Debug Line Helper should be initialized before been used");

  glUseProgram(ShaderIDPrimitives);

  glUniformMatrix4fv(get_uni_loc(ShaderIDPrimitives,"camera_modelview"), 1, false, camera.GetMatrixModelView().pointer());    PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(ShaderIDPrimitives,"camera_projection"), 1, false, camera.GetMatrixProjection().pointer());  PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ARRAY_BUFFER, vboPoints);          PRINT_OPENGL_ERROR();

  glEnableClientState(GL_VERTEX_ARRAY);              PRINT_OPENGL_ERROR();
  glVertexPointer(3, GL_FLOAT, 3*sizeof(float), 0);  PRINT_OPENGL_ERROR();

  glPointSize(3.0f);
  glDrawArrays(GL_POINTS, 0, NbPoints);              PRINT_OPENGL_ERROR();
  glPointSize(1.0f);
}

//-------------------------------------------------------
// Internal initialization of a line
//-------------------------------------------------------
void gltkDebugObject::InitializeLine(cpe::vec3 p1, cpe::vec3 p2)
{
  //Line data : Position - Color
  float const data[] = {
    p1.x(),p1.y(),p1.z(), 0.0f,1.0f,0.0f,
    p2.x(),p2.y(),p2.z(), 1.0f,0.0f,0.0f};

  //load shaders
  ShaderIDPrimitives = read_shader("DebugPrimitives.vert",
                             "DebugPrimitives.frag");

  if(vboLine==0)
  {
    glGenBuffers(1,&vboLine); PRINT_OPENGL_ERROR();
    ASSERT_CPE(vboLine != 0, "Problem initializing VBO");
  }
  //Send data on the GPU
  glBindBuffer(GL_ARRAY_BUFFER,vboLine);                               PRINT_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,4*3*sizeof(float),data,GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();
}

//-------------------------------------------------------
// Line Rendering Callback
//-------------------------------------------------------
void gltkDebugObject::RenderLine(gltkCamera const& camera) const
{
  if(!vboLine)
  {
    return;
  }
  ASSERT_CPE(vboLine != 0,"VBO for Debug Line Helper should be initialized before been used");

  glUseProgram(ShaderIDPrimitives);

  glUniformMatrix4fv(get_uni_loc(ShaderIDPrimitives,"camera_modelview"), 1, false, camera.GetMatrixModelView().pointer()); PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(ShaderIDPrimitives,"camera_projection"), 1, false, camera.GetMatrixProjection().pointer());    PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ARRAY_BUFFER, vboLine);                                         PRINT_OPENGL_ERROR();

  glEnableClientState(GL_VERTEX_ARRAY);                                           PRINT_OPENGL_ERROR();
  glVertexPointer(3, GL_FLOAT, 2*3*sizeof(float), 0);                             PRINT_OPENGL_ERROR();

  glEnableClientState(GL_COLOR_ARRAY);                                            PRINT_OPENGL_ERROR();
  glColorPointer(3, GL_FLOAT, 2*3*sizeof(float), buffer_offset(3*sizeof(float))); PRINT_OPENGL_ERROR();

  glLineWidth(3.0f);
  glDrawArrays(GL_LINES, 0, 2);                                                   PRINT_OPENGL_ERROR();
  glLineWidth(1.0f);
}

//-------------------------------------------------------
//  World Axis Rendering Callback
//-------------------------------------------------------
void gltkDebugObject::RenderAxis(gltkCamera const& camera) const
{
  glUseProgram(AxisHelper.shader_id());

  cpe::mat4 const orientation = cpe::mat4(camera.GetOrientation().to_mat3());
  cpe::mat4 const scaling = cpe::mat4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, camera.GetAspectRatio(), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

  glUniformMatrix4fv(get_uni_loc(AxisHelper.shader_id(),"camera_modelview"), 1, false, orientation.pointer()); PRINT_OPENGL_ERROR();
  glUniformMatrix4fv(get_uni_loc(AxisHelper.shader_id(),"camera_projection"), 1, false, scaling.pointer());    PRINT_OPENGL_ERROR();

  glClear(GL_DEPTH_BUFFER_BIT); PRINT_OPENGL_ERROR();

  glLineWidth(2.0);
  AxisHelper.draw();
  glLineWidth(1.0);
}
