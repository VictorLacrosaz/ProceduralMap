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

#ifndef MESH_IO_OBJ_HPP
#define MESH_IO_OBJ_HPP

#include <string>
#include <vector>

namespace cpe
{
class vec2;
class vec3;
}
class gltkMaterial;
class gltkGeometry;

/** Load a mesh structure from a OFF file */
gltkGeometry LoadOBJFromFile(std::string const& filename);

/** Load a material template library file. ("mtllib") */
std::vector<gltkMaterial> LoadMTLFromFile(std::string const& mtlFilename);

/** Internal structure following the definition of an obj file */
struct OBJFileStructure
{
  std::vector<cpe::vec3> Vertices;      //the coordinates of vertices
  std::vector<cpe::vec2> TCoords;       //the coordinates of textures (optional)
  std::vector<cpe::vec3> Normals;       //the coordinates of normals (optional)
  std::vector<int> MaterialIdx;         //the indices of materials (optional)
  std::vector<gltkMaterial> Materials;  //the materials used by the geometry (optional)


  std::vector<std::vector<int>> FaceVertexIdx;  //Polygon vertex indexes
  std::vector<std::vector<int>> FaceTCoordIdx; //Polygon texture coordinate indexes (optional)
  std::vector<std::vector<int>> FaceNormalIdx;  //Polygon normal indexes (optional)
  std::vector<int> FaceMaterialIdx;             //Polygon material indexes (optional)

  int CurrentMaterial = -1;  //Current face material
};

/** Split a given string of face f from obj style into a set of values.
 *
 *  ex. 1/2 4/5 7/8 -> 1 2 4 5 7 8
 *      1/2/3 4/4/1 4/7/8 -> 1 2 3 4 4 1 4 7 8
 *      1//3 4//1 7//9 -> 1 3 4 1 7 9
 *      4 7 8 4 -> 4 7 8 4
 *
*/
std::vector<int> OBJSplitFaceString(std::string const& face_data_str);

/** Internal read of an OBJ file. return an OBJ file structure. */
OBJFileStructure LoadOBJFileStructure(std::string const& filename);

/** Read the different OBJ file elements  */
void OBJReadVertex(std::stringstream& tokens, OBJFileStructure& obj); //"v"
void OBJReadTCoord(std::stringstream& tokens, OBJFileStructure& obj); //"vt"
void OBJReadNormal(std::stringstream& tokens, OBJFileStructure& obj); //"vn"
void OBJReadCurrentMaterial(std::stringstream& tokens, OBJFileStructure& obj); //"usemtl"
void OBJReadFace(std::stringstream& tokens, OBJFileStructure& obj); //"f"

void OBJLoadMTL(std::stringstream& tokens, OBJFileStructure& obj); //"mtllib"

#endif
