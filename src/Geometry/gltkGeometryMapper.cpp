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

#include "gltkGeometryMapper.hpp"

#include "error_handling.hpp"
#include "gltkGeometry.hpp"
#include "glUtils.hpp"


gltkGeometryMapper::gltkGeometryMapper()
  :vbo_vertex(0),vbo_normal(0),vbo_color(0),vbo_texture(0),vbo_material_id(0),vbo_index(0),number_of_triangles(0)
{}

gltkGeometryMapper::~gltkGeometryMapper()
{
  this->DeleteVBOs();
}


void gltkGeometryMapper::FillVBO(gltkGeometry const& m)
{
  if(m.IsValid() != true)
    throw cpe::exception_cpe("Mesh is considered as invalid, cannot fill vbo",EXCEPTION_PARAMETERS_CPE);

  number_of_triangles=m.GetTriangleCount();
  if(number_of_triangles<=0)
    throw cpe::exception_cpe("incorrect number of triangles",MACRO_EXCEPTION_PARAMETER);

  //create the new vbo
  if(vbo_vertex==0)
  {glGenBuffers(1,&vbo_vertex);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_vertex!=0,"Problem creation of VBO");

  if(vbo_normal==0)
  {glGenBuffers(1,&vbo_normal);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_normal!=0,"Problem creation of VBO");

  if(vbo_color==0)
  {glGenBuffers(1,&vbo_color);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_color!=0,"Problem creation of VBO");

  if(vbo_texture==0)
  {glGenBuffers(1,&vbo_texture);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_texture!=0,"Problem creation of VBO");

  if(vbo_material_id==0)
  {glGenBuffers(1,&vbo_material_id);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_material_id!=0,"Problem creation of VBO");

  if(vbo_index==0)
  {glGenBuffers(1,&vbo_index);PRINT_OPENGL_ERROR();}
  ASSERT_CPE(vbo_index!=0,"Problem creation of VBO");



  //VBO vertex
  glBindBuffer(GL_ARRAY_BUFFER,vbo_vertex); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_vertex))
    throw cpe::exception_cpe("vbo_buffer incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*m.GetVertexCount(),m.GetPointerVertex(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //VBO normal
  glBindBuffer(GL_ARRAY_BUFFER,vbo_normal); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_normal))
    throw cpe::exception_cpe("vbo_normal incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*m.GetNormalCount(),m.GetPointerNormal(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //VBO color
  glBindBuffer(GL_ARRAY_BUFFER,vbo_color); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_color))
    throw cpe::exception_cpe("vbo_color incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*m.GetColorCount(),m.GetPointerColor(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //VBO texture
  glBindBuffer(GL_ARRAY_BUFFER,vbo_texture); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_texture))
    throw cpe::exception_cpe("vbo_texture incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ARRAY_BUFFER,2*sizeof(float)*m.GetTCoordCount(),m.GetPointerTCoord(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //VBO texture
  glBindBuffer(GL_ARRAY_BUFFER,vbo_material_id); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_material_id))
    throw cpe::exception_cpe("vbo_material_id incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ARRAY_BUFFER,sizeof(int)*m.GetVertexCount(),m.GetPointerMaterialIndex(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //VBO index
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo_index); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_index))
    throw cpe::exception_cpe("vbo_index incorrect",EXCEPTION_PARAMETERS_CPE);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(int)*m.GetTriangleCount(),m.GetPointerTriangleIndex(),GL_DYNAMIC_DRAW); PRINT_OPENGL_ERROR();

  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0);           PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);   PRINT_OPENGL_ERROR();
}

void gltkGeometryMapper::DeleteVBOs()
{

  if(vbo_vertex!=0)
  {glDeleteBuffers(1,&vbo_vertex); PRINT_OPENGL_ERROR();}

  if(vbo_normal!=0)
  {glDeleteBuffers(1,&vbo_normal); PRINT_OPENGL_ERROR();}

  if(vbo_color!=0)
  {glDeleteBuffers(1,&vbo_color); PRINT_OPENGL_ERROR();}

  if(vbo_texture!=0)
  {glDeleteBuffers(1,&vbo_texture); PRINT_OPENGL_ERROR();}

  if(vbo_material_id!=0)
  {glDeleteBuffers(1,&vbo_material_id); PRINT_OPENGL_ERROR();}


  if(vbo_index!=0)
  {glDeleteBuffers(1,&vbo_index); PRINT_OPENGL_ERROR();}
}

void gltkGeometryMapper::Render() const
{
  if(number_of_triangles<=0)
    throw cpe::exception_cpe("Incorrect number of triangles",EXCEPTION_PARAMETERS_CPE);


  glBindBuffer(GL_ARRAY_BUFFER,vbo_vertex); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_vertex))
    throw cpe::exception_cpe("vbo_vertex incorrect",EXCEPTION_PARAMETERS_CPE);
  glEnableVertexAttribArray(0);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ARRAY_BUFFER,vbo_normal); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_normal))
    throw cpe::exception_cpe("vbo_normal incorrect",EXCEPTION_PARAMETERS_CPE);
  glEnableVertexAttribArray(1);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER,vbo_color); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_color))
    throw cpe::exception_cpe("vbo_color incorrect",EXCEPTION_PARAMETERS_CPE);
  glEnableVertexAttribArray(2);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ARRAY_BUFFER,vbo_texture); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_texture))
    throw cpe::exception_cpe("vbo_texture incorrect",EXCEPTION_PARAMETERS_CPE);
  glEnableVertexAttribArray(3);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);                                           PRINT_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER,vbo_material_id); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_material_id))
    throw cpe::exception_cpe("vbo_material_id incorrect",EXCEPTION_PARAMETERS_CPE);
  glEnableVertexAttribArray(4);                                                                    PRINT_OPENGL_ERROR();
  glVertexAttribIPointer(4, 1, GL_INT, 0, 0);                                           PRINT_OPENGL_ERROR();


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo_index); PRINT_OPENGL_ERROR();
  if(!glIsBuffer(vbo_index))
    throw cpe::exception_cpe("vbo_index incorrect",EXCEPTION_PARAMETERS_CPE);
  glDrawElements(GL_TRIANGLES, 3*number_of_triangles, GL_UNSIGNED_INT, 0); PRINT_OPENGL_ERROR();

  //Disable vertex arrays to allow drawing with old OpenGL (< 1.2)
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);

  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0);           PRINT_OPENGL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);   PRINT_OPENGL_ERROR();
}



void gltkGeometryMapper::UpdateVertexVBO(gltkGeometry const& m)
{
  //VBO vertex
  glBindBuffer(GL_ARRAY_BUFFER,vbo_vertex); PRINT_OPENGL_ERROR();
  ASSERT_CPE(glIsBuffer(vbo_vertex),"vbo_buffer incorrect");

  glBufferSubData(GL_ARRAY_BUFFER,0,3*sizeof(float)*m.GetVertexCount(),m.GetPointerVertex()); PRINT_OPENGL_ERROR();
  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0);           PRINT_OPENGL_ERROR();
}

void gltkGeometryMapper::UpdateNormalVBO(gltkGeometry const& m)
{
  //VBO vertex
  glBindBuffer(GL_ARRAY_BUFFER,vbo_normal); PRINT_OPENGL_ERROR();
  ASSERT_CPE(glIsBuffer(vbo_normal),"vbo_buffer incorrect");

  glBufferSubData(GL_ARRAY_BUFFER,0,3*sizeof(float)*m.GetNormalCount(),m.GetPointerNormal()); PRINT_OPENGL_ERROR();

  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0);           PRINT_OPENGL_ERROR();
}

void gltkGeometryMapper::UpdateColorVBO(gltkGeometry const& m)
{
  //VBO vertex
  glBindBuffer(GL_ARRAY_BUFFER,vbo_color); PRINT_OPENGL_ERROR();
  ASSERT_CPE(glIsBuffer(vbo_color),"vbo_buffer incorrect");

  glBufferSubData(GL_ARRAY_BUFFER,0,3*sizeof(float)*m.GetColorCount(),m.GetPointerColor()); PRINT_OPENGL_ERROR();

  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0);           PRINT_OPENGL_ERROR();
}

void gltkGeometryMapper::UpdateTCoordVBO(gltkGeometry const& m)
{
  //VBO vertex
  glBindBuffer(GL_ARRAY_BUFFER,vbo_texture); PRINT_OPENGL_ERROR();
  ASSERT_CPE(glIsBuffer(vbo_texture),"vbo_buffer incorrect");

  glBufferSubData(GL_ARRAY_BUFFER,0,3*sizeof(float)*m.GetTCoordCount(),m.GetPointerTCoord()); PRINT_OPENGL_ERROR();

  //Unbind buffer for 2d Hub drawing
  glBindBuffer(GL_ARRAY_BUFFER,0); PRINT_OPENGL_ERROR();
}
