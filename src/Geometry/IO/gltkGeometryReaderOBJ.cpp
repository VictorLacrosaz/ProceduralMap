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

#include "gltkGeometryReaderOBJ.hpp"
#include "ProjectConfig.h"

#include "error_handling.hpp"
#include "gltkGeometry.hpp"
#include "gltkMaterial.hpp"
#include "triangle_index.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <sstream>
#include <fstream>
#include <assert.h>
#include <map>


std::vector<int> OBJSplitFaceString(std::string const& face_data_str)
{
  std::vector<int> data;
  std::string current_str=face_data_str;

  while(current_str.length()>0)
  {
    while(current_str.length()>0 && current_str[0]=='/')
    {
      std::string temp_str=current_str.substr(1,current_str.length()-1);
      current_str=temp_str;
    }

    int value;
    std::stringstream tokenizer(current_str);
    tokenizer>>value;
    data.push_back(value);

    std::string residual;
    tokenizer>>residual;

    current_str=residual;
  }

  return data;
}

void OBJReadVertex(std::stringstream& tokens,OBJFileStructure& obj)
{
  cpe::vec3 v;
  tokens >> v.x();
  tokens >> v.y();
  tokens >> v.z();
  obj.Vertices.push_back(v);
}

void OBJReadTCoord(std::stringstream& tokens, OBJFileStructure& obj)
{
  cpe::vec2 t;
  tokens >> t.x();
  tokens >> t.y();
  obj.TCoords.push_back(t);
}

void OBJReadNormal(std::stringstream& tokens, OBJFileStructure& obj)
{
  cpe::vec3 n;
  tokens >> n.x();
  tokens >> n.y();
  tokens >> n.z();
  obj.Normals.push_back(n);
}

void OBJReadCurrentMaterial(std::stringstream& tokens, OBJFileStructure& obj)
{
  std::string mtlName;
  tokens >> mtlName;

  //Check for "None" material
  if(mtlName == "None")
  {
    obj.CurrentMaterial = -1;
    return;
  }

  for(unsigned int i = 0; i < obj.Materials.size(); i++)
  {
    if(obj.Materials[i].GetName() == mtlName)
    {
      obj.CurrentMaterial = i;
      break;
    }
    else if(i == obj.Materials.size() - 1)
    {
      throw cpe::exception_cpe("Cannot find material " + mtlName, MACRO_EXCEPTION_PARAMETER);
    }
  }
}

void OBJLoadMTL(std::stringstream& tokens, OBJFileStructure& obj)
{
  std::string mtlFileName;
  tokens >> mtlFileName;
  obj.Materials = LoadMTLFromFile(mtlFileName);
}

void OBJReadFace(std::stringstream& tokens, OBJFileStructure& obj)
{
  std::vector<std::vector<int> > face_data;

  //WARNING: Should stop at 3 to handle only triangles
  while(tokens.good())
  {
    std::string polygon_index_str;
    tokens >> polygon_index_str;

    std::vector<int> const current_data = OBJSplitFaceString(polygon_index_str);
    face_data.push_back(current_data);
  }

  std::vector<int> temp_vertex;
  std::vector<int> temp_texture;
  std::vector<int> temp_normal;

  for(auto const& index_face : face_data)
  {
    int const N_dim=index_face.size();
    for(int k_dim=0;k_dim<N_dim;++k_dim)
    {
      int const value=index_face[k_dim];

      switch(k_dim)
      {
        case 0:
          temp_vertex.push_back(value-1);
          break;
        case 1:
          temp_texture.push_back(value-1);
          break;
        case 2:
          temp_normal.push_back(value-1);
          break;
        default:
          std::cerr<<"Error reading file "<<std::endl;
          exit(1);
      }
    }
  }

  if(temp_vertex.size() > 0)
  {
    obj.FaceVertexIdx.push_back(temp_vertex);
    //Associate the current material to the current vertex index
    obj.FaceMaterialIdx.push_back(obj.CurrentMaterial);
  }
  if(temp_texture.size() > 0)
  {
    obj.FaceTCoordIdx.push_back(temp_texture);
  }
  if(temp_normal.size() > 0)
  {
    obj.FaceNormalIdx.push_back(temp_normal);
  }
}

OBJFileStructure LoadOBJFileStructure(std::string const& filename)
{
  std::ifstream fid(filename.c_str());
  if(!fid.good())
  {
    throw cpe::exception_cpe("Cannot open file "+filename,MACRO_EXCEPTION_PARAMETER);
  }
  std::string buffer;
  OBJFileStructure structure;

  //read the whole file
  while(fid.good() == true)
  {
    //read line
    std::getline(fid, buffer);

    if(buffer.size() > 0)
    {
      std::stringstream tokens_buffer(buffer);
      std::string first_word;
      tokens_buffer>>first_word;

      //skip comments
      if(first_word.length()>0 && first_word[0]!='#')
      {
        //load materials MTL file
        if(first_word=="mtllib")
        {
          OBJLoadMTL(tokens_buffer, structure);
        }
        //material
        if(first_word == "usemtl")
        {
          OBJReadCurrentMaterial(tokens_buffer, structure);
        }
        //vertices
        if(first_word == "v")
        {
          OBJReadVertex(tokens_buffer, structure);
        }
        //texture
        if(first_word == "vt")
        {
          OBJReadTCoord(tokens_buffer, structure);
        }
        //normal
        if(first_word == "vn")
        {
          OBJReadNormal(tokens_buffer, structure);
        }
        //connectivity
        if(first_word == "f")
        {
          OBJReadFace(tokens_buffer,structure);
        }
      }
    }
  }

  //Close file
  fid.close();

  return structure;
}

gltkGeometry LoadOBJFromFile(const std::string& filename)
{
  gltkGeometry mesh_loaded;

  OBJFileStructure obj=LoadOBJFileStructure(filename);

  bool is_vertex=obj.Vertices.size()>0 && obj.FaceVertexIdx.size()>0;
  bool is_normal=obj.Normals.size()>0 && obj.FaceNormalIdx.size()>0;
  bool is_texture=obj.TCoords.size()>0 && obj.FaceTCoordIdx.size()>0;


  if(is_vertex && !is_normal && !is_texture)
  {
    int const N_face=obj.FaceVertexIdx.size();
    for(int k_face=0;k_face<N_face;++k_face)
    {
      auto const& polygon=obj.FaceVertexIdx[k_face];
      int const dim=polygon.size();

      assert(dim>=2);
      for(int k=2;k<dim;++k)
      {
        mesh_loaded.AddTriangleIndex(cpe::triangle_index(polygon[0],polygon[1],polygon[k]));
      }
    }

    int const N_vertex=obj.Vertices.size();
    for(int k_vertex=0;k_vertex<N_vertex;++k_vertex)
    {
      mesh_loaded.AddVertex(obj.Vertices[k_vertex]);
    }
  }

  if((is_vertex && !is_normal && is_texture) || (is_vertex && is_normal && is_texture) )//note: normal are not read
  {
    std::map<int,int> map_vertex;
    std::map<int,int> map_texture;

    long int counter_max=0;

    int const N_face=obj.FaceVertexIdx.size();
    assert(static_cast<int>(obj.FaceTCoordIdx.size())==N_face);
    for(int k_face=0;k_face<N_face;++k_face)
    {
      std::vector<int> const& face_vertex=obj.FaceVertexIdx[k_face];
      std::vector<int> const& face_texture=obj.FaceTCoordIdx[k_face];
      int face_material=obj.FaceMaterialIdx[k_face];

      int const dim=face_vertex.size();
      assert(dim>2);
      assert(static_cast<int>(face_texture.size())==dim);
      for(int k_dim=0;k_dim<dim;++k_dim)
      {
        int const idx_vertex=face_vertex[k_dim];
        int const idx_texture=face_texture[k_dim];

        std::map<int,int>::const_iterator it_vertex=map_vertex.find(idx_vertex);
        std::map<int,int>::const_iterator it_texture=map_texture.find(idx_texture);

        if(it_vertex==map_vertex.end() || it_texture==map_texture.end())
        {
          map_vertex[idx_vertex]=counter_max;
          map_texture[idx_texture]=counter_max;

          counter_max++;

          assert(static_cast<int>(obj.Vertices.size())>idx_vertex);
          assert(static_cast<int>(obj.TCoords.size())>idx_texture);

          mesh_loaded.AddVertex(obj.Vertices[idx_vertex]);
          mesh_loaded.AddTCoord(obj.TCoords[idx_texture]);
          mesh_loaded.AddMaterialIndex(face_material);
        }
      }

      int const index_0=map_vertex[face_vertex[0]];
      for(int k_dim=1;k_dim<dim-1;++k_dim)
      {
        int const index_1=map_vertex[face_vertex[k_dim]];
        int const index_2=map_vertex[face_vertex[k_dim+1]];
        mesh_loaded.AddTriangleIndex(cpe::triangle_index(index_0,index_1,index_2));
      }
    }
  }

  mesh_loaded.FillEmptyFields();
  mesh_loaded.SetMaterials(obj.Materials);

  ASSERT_CPE(mesh_loaded.IsValid(),"Mesh is invalid");

  return mesh_loaded;

}

std::vector<gltkMaterial> LoadMTLFromFile(std::string const& mtlFilename)
{
  std::string data_dir = DATA_DIR;
  data_dir.append( "/" );

  std::string filename = data_dir + mtlFilename;

  std::ifstream fid(filename.c_str());
  if(!fid.good())
  {
    throw cpe::exception_cpe("Cannot open file "+filename, MACRO_EXCEPTION_PARAMETER);
  }
  std::string buffer;

  std::vector<gltkMaterial> mtlVector;

  //read the whole file
  while(fid.good()==true)
  {
    //read line
    std::getline(fid,buffer);

    if(buffer.size()>0)
    {
      std::stringstream tokens_buffer(buffer);
      std::string first_word;
      tokens_buffer>>first_word;

      //skip comments
      if(first_word.length()>0 && first_word[0]!='#')
      {
        //New material
        if(first_word=="newmtl")
        {
          mtlVector.push_back(gltkMaterial());
          std::string mtlName;
          tokens_buffer >> mtlName;
          mtlVector[mtlVector.size()-1].SetName(mtlName);
        }
        //Kd
        if(first_word=="Kd")
        {
          cpe::vec3 Kd;
          tokens_buffer >> Kd.x();
          tokens_buffer >> Kd.y();
          tokens_buffer >> Kd.z();
          mtlVector[mtlVector.size()-1].SetKd(Kd) ;
        }
        //Ka
        if(first_word=="Ka")
        {
          cpe::vec3 Ka;
          tokens_buffer >> Ka.x();
          tokens_buffer >> Ka.y();
          tokens_buffer >> Ka.z();
          mtlVector[mtlVector.size()-1].SetKa(Ka) ;
        }
        //Texture
        if(first_word == "map_Kd")
        {
          std::string filename;
          tokens_buffer >> filename;
          mtlVector[mtlVector.size()-1].SetKdTexture(filename);
        }
      }
    }
  }

  fid.close();
  return mtlVector;
}
