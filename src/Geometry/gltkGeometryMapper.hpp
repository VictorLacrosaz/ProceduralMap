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

#ifndef GLTK_GEOMETRY_MAPPER_HPP
#define GLTK_GEOMETRY_MAPPER_HPP

#include "GL/glew.h"
#include "GL/gl.h"


class gltkGeometry;

/** Class to manipulate meshes to be drawn by opendGL */
class gltkGeometryMapper
{
public:

    gltkGeometryMapper();
    ~gltkGeometryMapper();

    /** Send the mesh data to the VBO, setup all vbos*/
    void FillVBO(gltkGeometry const& m);
    /** Ask the GPU to draw the data.
     *  fill_vbo must have been called previously */
    void Render() const;


    /** Update only the vertex on the GPU */
    void UpdateVertexVBO(gltkGeometry const& m);
    /** Update only the normal on the GPU */
    void UpdateNormalVBO(gltkGeometry const& m);
    /** Update only the color on the GPU */
    void UpdateColorVBO(gltkGeometry const& m);
    /** Update only the texture on the GPU */
    void UpdateTCoordVBO(gltkGeometry const& m);

private:

    /** Helper function to delete the vbos */
    void DeleteVBOs();

    /** VBO for the vertices */
    GLuint vbo_vertex;
    /** VBO for the normals */
    GLuint vbo_normal;
    /** VBO for the colors */
    GLuint vbo_color;
    /** VBO for the texture coords */
    GLuint vbo_texture;
    /** VBO for the texture coords */
    GLuint vbo_material_id;
    /** VBO for the triangle index */
    GLuint vbo_index;

    /** Store the number of triangles of the mesh */
    unsigned int number_of_triangles;
};

#endif
