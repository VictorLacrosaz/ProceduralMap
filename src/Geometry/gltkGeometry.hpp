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

#ifndef GLTK_GEOMETRY_HPP
#define GLTK_GEOMETRY_HPP

#include <vector>

#include "mat3.hpp"
#include "mat4.hpp"
#include "triangle_index.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "gltkMaterial.hpp"

/** Basic container for a triangular mesh structure.
 * Used as a parent class for other mesh classes.
 * The mesh contains:
 * - a vector of 3D vertices
 * - 1 normal per vertex
 * - 1 color (r,g,b) per vertex
 * - 1 texture coordinate (u,v) per vertex
 * - 1 material index (i) per vertex
*/
class gltkGeometry
{
public:

    gltkGeometry();

    /******************************************/
    // Size
    /******************************************/

    /** Give the number of vertices */
    int GetVertexCount() const;
    /** Give the number of normals */
    int GetNormalCount() const;
    /** Give the number of colors */
    int GetColorCount() const;
    /** Give the number of texture coordinates */
    int GetTCoordCount() const;
    /** Give the number of material indexes */
    int GetMaterialIndexCount() const;
    /** Give the number of triangles */
    int GetTriangleCount() const;
    /** Give the number of Material object used by the geometry */
    int GetNumberOfMaterials() const;

    /******************************************/
    // FILLING
    /******************************************/

    /** Fill the mesh with a uniform color (r,g,v). Each component is between [0,1]. */
    void FillColor(cpe::vec3 const& c);
    /** Fill the mesh with color depending on its (x,y,z) coordinates. */
    void FillColorXYZ();
    /** Fill the mesh with color depending on its normal coordinates. */
    void FillColorNormal();

    /** Fill automatically the normals of the mesh. */
    void FillNormal();

    /** Fill automatically the material indexes of the mesh. */
    void FillMaterialIdx(int const idx);

    /** Fill all the fields (normal, color, texture, etc) if they are not already filled. */
    void FillEmptyFields();


    /******************************************/
    // Deformations
    /******************************************/

    /** Inverse all the normals */
    void InverseNormals();
    /** Multiply all the vertices by the given matrix */
    void ApplyTransform(cpe::mat3 const& T);
    /** Multiply all the vertices by the given matrix */
    void ApplyTransform(cpe::mat4 const& T);
    /** Center the mesh around the origin and set its maximal size in x,y or z direction to 1. */
    void ScaleAndCenter();
    /** Scale the mesh */
    void ApplyScale(float s);
    /** Scale the mesh */
    void ApplyScale(float sx,float sy,float sz);
    /** Translate the mesh */
    void ApplyTranslation(cpe::vec3 const& t);
    /** Rotate the mesh */
    void ApplyRotation(cpe::vec3 const& axis,float angle);

    /******************************************/
    // Pointers
    /******************************************/

    /** Get a pointer on the vertices (for OpenGL) */
    float const* GetPointerVertex() const;
    /** Get a pointer on the normals (for OpenGL) */
    float const* GetPointerNormal() const;
    /** Get a pointer on the color (for OpenGL) */
    float const* GetPointerColor() const;
    /** Get a pointer on the textures (for OpenGL) */
    float const* GetPointerTCoord() const;
    /** Get a pointer to the material indexes */
    int const* GetPointerMaterialIndex() const;
    /** Get a pointer to the indices of the triangles (for OpenGL) */
    int const* GetPointerTriangleIndex() const;


    bool IsValid() const;


    cpe::vec3 GetVertex(int index) const;
    void SetVertex(int index, cpe::vec3 const& value);
    cpe::vec3 GetNormal(int index) const;
    void SetNormal(int index, cpe::vec3 const& value);
    cpe::vec3 GetColor(int index) const;
    void SetColor(int index, cpe::vec3 const& value);
    cpe::vec2 GetTCoord(int index) const;
    void SetTCoord(int index, cpe::vec2 const& value);
    int GetMaterialIdx(int index) const;
    void SetMaterialIdx(int index, int value);
    cpe::triangle_index GetTriangleIdx(int index) const;
    void SetTriangleIdx(int index, cpe::triangle_index const& value);

    std::vector<gltkMaterial> GetMaterials() const;
    void SetMaterials(std::vector<gltkMaterial> materials);


    void AddVertex(cpe::vec3 const& v);
    void AddNormal(cpe::vec3 const& n);
    void AddColor(cpe::vec3 const& c);
    void AddTCoord(cpe::vec2 const& t);
    void AddMaterialIndex(int const idx);
    void AddTriangleIndex(cpe::triangle_index const& idx);

    /** Compute the two extremities of the Axis Aligned Bounding Box */
    void ComputeAABB(cpe::vec3& corner_min,cpe::vec3& corner_max);


private:

    /** Internal storage for the vertices */
    std::vector<cpe::vec3> Vertices;
    /** Internal storage for the normals */
    std::vector<cpe::vec3> Normals;
    /** Internal storage for the colors */
    std::vector<cpe::vec3> Colors;
    /** Internal storage for the texture coordinates */
    std::vector<cpe::vec2> TCoords;
    /** Internal storage for the vertex material indexes */
    std::vector<int> MaterialIdx;

    /** Internal storage for the triangles indices */
    std::vector<cpe::triangle_index> TriangleIdx;

    /** Internal storage for the materials */
    std::vector<gltkMaterial> Materials;

};

#endif
