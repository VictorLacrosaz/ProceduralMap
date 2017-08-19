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

#include "gltkGeometry.hpp"

#include "error_handling.hpp"

#include <cmath>


gltkGeometry::gltkGeometry()
  :Vertices(), Normals(), Colors(), TCoords(), MaterialIdx(), TriangleIdx()
{}

int gltkGeometry::GetVertexCount() const {return Vertices.size();}
int gltkGeometry::GetNormalCount() const {return Normals.size();}
int gltkGeometry::GetColorCount() const {return Colors.size();}
int gltkGeometry::GetTCoordCount() const {return TCoords.size();}
int gltkGeometry::GetMaterialIndexCount() const {return MaterialIdx.size();}
int gltkGeometry::GetTriangleCount() const {return TriangleIdx.size();}
int gltkGeometry::GetNumberOfMaterials() const {return Materials.size();}

cpe::vec3 gltkGeometry::GetVertex(int const index) const
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetVertexCount(),"Index ("+std::to_string(index)+") must be less than the current size of the vertices ("+std::to_string(this->GetVertexCount())+")");

  return Vertices[index];
}
void gltkGeometry::SetVertex(int index, cpe::vec3 const& value)
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetVertexCount(),"Index ("+std::to_string(index)+") must be less than the current size of the vertices ("+std::to_string(this->GetVertexCount())+")");

  Vertices[index] = value;
}

cpe::vec3 gltkGeometry::GetNormal(int const index) const
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetNormalCount(),"Index ("+std::to_string(index)+") must be less than the current size of the normals ("+std::to_string(this->GetNormalCount())+")");

  return Normals[index];
}
void gltkGeometry::SetNormal(int index, cpe::vec3 const& value)
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetNormalCount(),"Index ("+std::to_string(index)+") must be less than the current size of the normals ("+std::to_string(this->GetNormalCount())+")");

  Normals[index] = value;
}

cpe::vec3 gltkGeometry::GetColor(int const index) const
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetColorCount(),"Index ("+std::to_string(index)+") must be less than the current size of the colors ("+std::to_string(this->GetColorCount())+")");

  return Colors[index];
}
void gltkGeometry::SetColor(int index, cpe::vec3 const& value)
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetColorCount(),"Index ("+std::to_string(index)+") must be less than the current size of the colors ("+std::to_string(this->GetColorCount())+")");

  Colors[index] = value;
}

cpe::vec2 gltkGeometry::GetTCoord(int const index) const
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetTCoordCount(),"Index ("+std::to_string(index)+") must be less than the current size of the texture coordinates ("+std::to_string(this->GetTCoordCount())+")");

  return TCoords[index];
}
void gltkGeometry::SetTCoord(int index, cpe::vec2 const& value)
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetTCoordCount(),"Index ("+std::to_string(index)+") must be less than the current size of the texture coordinates ("+std::to_string(this->GetTCoordCount())+")");

  TCoords[index] = value;
}

int gltkGeometry::GetMaterialIdx(int const index) const
{
  ASSERT_CPE(index>=0, "Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetMaterialIndexCount(), "Index ("+std::to_string(index)+") must be less than the current size of the connectivity ("+std::to_string(this->GetTriangleCount())+")");

  return MaterialIdx[index];
}
void gltkGeometry::SetMaterialIdx(int const index, int const value)
{
  ASSERT_CPE(index >= 0, "Index (" +std::to_string(index)+") must be positive");
  ASSERT_CPE(index < this->GetMaterialIndexCount(), "Index ("+std::to_string(index)+") must be less than the current size of the connectivity ("+std::to_string(this->GetTriangleCount())+")");

  MaterialIdx[index] = value;
}

cpe::triangle_index gltkGeometry::GetTriangleIdx(int const index) const
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetTriangleCount(),"Index ("+std::to_string(index)+") must be less than the current size of the connectivity ("+std::to_string(this->GetTriangleCount())+")");

  return TriangleIdx[index];
}
void gltkGeometry::SetTriangleIdx(int const index, cpe::triangle_index const& value)
{
  ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
  ASSERT_CPE(index<this->GetTriangleCount(),"Index ("+std::to_string(index)+") must be less than the current size of the connectivity ("+std::to_string(this->GetTriangleCount())+")");

  TriangleIdx[index] = value;
}

std::vector<gltkMaterial> gltkGeometry::GetMaterials() const
{
  return Materials;
}
void gltkGeometry::SetMaterials(std::vector<gltkMaterial> materials)
{
  Materials = materials;
}

void gltkGeometry::AddVertex(cpe::vec3 const& v)
{
  Vertices.push_back(v);
}

void gltkGeometry::AddNormal(cpe::vec3 const& n)
{
  Normals.push_back(n);
}

void gltkGeometry::AddColor(cpe::vec3 const& c)
{
  Colors.push_back(c);
}

void gltkGeometry::AddTCoord(cpe::vec2 const& t)
{
  TCoords.push_back(t);
}

void gltkGeometry::AddMaterialIndex(int const idx)
{
  MaterialIdx.push_back(idx);
}

void gltkGeometry::AddTriangleIndex(cpe::triangle_index const& idx)
{
  TriangleIdx.push_back(idx);
}


void gltkGeometry::FillColor(cpe::vec3 const& c)
{
  if(this->GetColorCount()!=this->GetVertexCount())
    Colors.resize(this->GetVertexCount());

  for(auto& col : Colors)
    col=c;
}

void gltkGeometry::FillColorXYZ()
{
  int const N=this->GetVertexCount();

  if(this->GetColorCount()!=N)
    Colors.resize(N);

  cpe::vec3 p_min, p_max;
  this->ComputeAABB(p_min, p_max);

  //diagonal linking the two extremities
  cpe::vec3 d=p_max-p_min;

  //ensure a non zero size
  for(int k_dim=0;k_dim<3;++k_dim)
    if(d[k_dim]<1e-6)
      d[k_dim]=1.0f;

  //fill the color vector
  for(int k=0;k<N;++k)
  {
    //normalize the color (black at p_min, and white at p_max)
    cpe::vec3 p = this->GetVertex(k)-p_min;
    this->SetColor(k, cpe::vec3(p.x()/d.x(), p.y()/d.y(), p.z()/d.z()));
  }

}

void gltkGeometry::FillColorNormal()
{
  int const N=this->GetVertexCount();

  if(this->GetColorCount()!=N)
    Colors.resize(N);

  //fill the color value with c=|n|
  for(int k=0;k<N;++k)
  {
    cpe::vec3 const& n = this->GetNormal(k);
    this->SetColor(k, cpe::vec3(std::abs(n.x()), std::abs(n.y()), std::abs(n.z())));
  }
}

void gltkGeometry::FillMaterialIdx(int const idx)
{
  ASSERT_CPE(idx < this->GetNumberOfMaterials(), "Material index out of bounds.");

  int const N=this->GetVertexCount();

  if(this->GetMaterialIndexCount() != N)
  {
    MaterialIdx.resize(N);
  }

  //fill the material indexes with idx
  for(int k=0; k<N; ++k)
  {
    this->SetMaterialIdx(k, idx);
  }
}

void gltkGeometry::FillNormal()
{
  int const N_vertex=this->GetVertexCount();
  if(this->GetNormalCount()!=N_vertex)
    Normals.resize(N_vertex);

  //init normal data to 0
  for(auto& n : Normals)
    n=cpe::vec3();

  //walk through all the triangles and add each triangle normal to the vertices
  int const N_triangle=this->GetTriangleCount();
  for(int k_triangle=0;k_triangle<N_triangle;++k_triangle)
  {
    //get current triangle index
    cpe::triangle_index const& tri = this->GetTriangleIdx(k_triangle);

    //check that the index given have correct values
    ASSERT_CPE(tri.u0()>=0 && tri.u0()<N_vertex,"Incorrect triangle index");
    ASSERT_CPE(tri.u1()>=0 && tri.u1()<N_vertex,"Incorrect triangle index");
    ASSERT_CPE(tri.u2()>=0 && tri.u2()<N_vertex,"Incorrect triangle index");

    //compute current normal
    cpe::vec3 const& p0=this->GetVertex(tri.u0());
    cpe::vec3 const& p1=this->GetVertex(tri.u1());
    cpe::vec3 const& p2=this->GetVertex(tri.u2());

    cpe::vec3 const u1=normalized(p1-p0);
    cpe::vec3 const u2=normalized(p2-p0);
    cpe::vec3 const n=normalized(cross(u1,u2));

    //add the computed normal to the Normals
    for(int kv=0;kv<3;++kv)
      Normals[tri[kv]] += n;
  }

  //normalize all normal value
  for(auto& n : Normals)
  {
    n = normalized(n);
  }
}

void gltkGeometry::InverseNormals()
{
  for(auto& n : Normals)
  {
    n = -n;
  }
}

void gltkGeometry::ApplyTransform(cpe::mat3 const& T)
{
  for(auto& p : Vertices)
  {
    p = T * p;
  }
}

void gltkGeometry::ApplyTransform(cpe::mat4 const& T)
{
  for(auto& p : Vertices)
  {
    p = T * p;
  }
}

void gltkGeometry::ScaleAndCenter()
{
  cpe::vec3 corner_min, corner_max;
  this->ComputeAABB(corner_min, corner_max);
  cpe::vec3 const center = (corner_min + corner_max)/2;
  cpe::vec3 const d = corner_max - corner_min;

  this->ApplyTranslation(-center);
  float const s = std::max(std::max(d.x(), d.y()), d.z());
  if(s > 1e-6f)
  {
    this->ApplyScale(1.0f/s);
  }
}

void gltkGeometry::ApplyScale(float const s)
{
  for(auto& p : Vertices)
  {
    p = s * p;
  }
}

void gltkGeometry::ApplyScale(float const sx, float const sy, float const sz)
{
  for(cpe::vec3& p : Vertices)
  {
    p.x() *= sx;
    p.y() *= sy;
    p.z() *= sz;
  }
}

void gltkGeometry::ApplyTranslation(cpe::vec3 const& t)
{
  for(cpe::vec3& p : Vertices)
  {
    p += t;
  }
}

void gltkGeometry::ApplyRotation(cpe::vec3 const& axis, float const angle)
{
  cpe::mat3 R;
  R.set_rotation(axis, angle);
  for(cpe::vec3& p : Vertices)
  {
    p = R * p;
  }
}

bool gltkGeometry::IsValid() const
{
  //gltkGeometry should have at least one vertex
  if(this->GetVertexCount()<=0)
  {
    std::cout<<"gltkGeometry has 0 vertex"<<std::endl;
    return false;
  }
  //gltkGeometry should have at least one triangle
  if(this->GetTriangleCount()<=0)
  {
    std::cout<<"Connectivity has size 0";
    return false;
  }

  //gltkGeometry should identical size for vertex, normal, color and texture_coord
  if(this->GetVertexCount()!=this->GetNormalCount())
  {
    std::cout<<"Vertex count differs from normal count"<<std::endl;
    return false;
  }
  if(this->GetVertexCount()!=this->GetColorCount())
  {
    std::cout<<"Vertex count differs from Color count"<<std::endl;
    return false;
  }
  if(this->GetVertexCount()!=this->GetTCoordCount())
  {
    std::cout<<"Vertex count differs from TCoord count"<<std::endl;
    return false;
  }
  if(this->GetVertexCount()!=this->GetMaterialIndexCount())
  {
    std::cout<<"Vertex count differs from material idx count"<<std::endl;
    return false;
  }

  //vertices should not have too large coordinates
  for(int k=0,N=this->GetVertexCount();k<N;++k)
  {
    cpe::vec3 const& p=Vertices[k];
    if(std::abs(p.x())>50000 || std::abs(p.y())>50000 || std::abs(p.z())>50000)
    {
      std::cout<<"Vertex "<<k<<" ("<<p<<") has very large size"<<std::endl;
      return false;
    }
  }

  //normals should be normalized
  for(int k=0,N=this->GetNormalCount();k<N;++k)
  {
    cpe::vec3 const& n=Normals[k];
    if(std::abs(norm(n)-1.0f)>1e-6f)
    {
      std::cout<<"Normal "<<k<<"("<<n<<") is not normalized"<<std::endl;
      return false;
    }
  }

  //color components should be in [0,1]
  for(int k=0,N=this->GetColorCount();k<N;++k)
  {
    cpe::vec3 const& c=Colors[k];
    if(c.x()<0 || c.x()>1 || c.y()<0 || c.y()>1 || c.z()<0 || c.z()>1)
    {
      std::cout<<"Color "<<k<<"("<<c<<") has values outside [0,1]"<<std::endl;
      return false;
    }
  }

  //all triangle indices should be in the vertex list with non degenerated triangle
  int const N_vertex=this->GetVertexCount();
  for(int k=0,N=this->GetTriangleCount();k<N;++k)
  {
    cpe::triangle_index const& tri = TriangleIdx[k];
    int const u0=tri.u0();
    int const u1=tri.u1();
    int const u2=tri.u2();

    if(u0==u1 || u0==u2 || u1==u2)
    {
      std::cout<<"Triangle index "<<k<<" ("<<tri<<") is degenerated"<<std::endl;
      return false;
    }

    if(u0<0 || u1<0 || u2<0 || u0>=N_vertex || u1>=N_vertex || u2>=N_vertex)
    {
      std::cout<<"Triangle index "<<k<<" ("<<tri<<") has incorrect value with respect to the current vertex list of size "<<N_vertex<<std::endl;
      return false;
    }
  }

  return true;
}

float const* gltkGeometry::GetPointerVertex() const
{
  ASSERT_CPE(this->GetVertexCount() > 0, "No vertex");
  return Vertices[0].pointer();
}
float const* gltkGeometry::GetPointerNormal() const
{
  ASSERT_CPE(this->GetNormalCount() > 0, "No normal");
  return Normals[0].pointer();
}
float const* gltkGeometry::GetPointerColor() const
{
  ASSERT_CPE(this->GetColorCount() > 0, "No color");
  return Colors[0].pointer();
}
float const* gltkGeometry::GetPointerTCoord() const
{
  ASSERT_CPE(this->GetTCoordCount() > 0, "No texture coord");
  return TCoords[0].pointer();
}
int const* gltkGeometry::GetPointerMaterialIndex() const
{
  ASSERT_CPE(this->GetMaterialIndexCount() > 0, "No material index");
  return &MaterialIdx[0];
}
int const* gltkGeometry::GetPointerTriangleIndex() const
{
  ASSERT_CPE(this->GetTriangleCount() > 0, "No connectivity");
  return TriangleIdx[0].pointer();
}

void gltkGeometry::FillEmptyFields()
{
  int const N_vertex = this->GetVertexCount();

  if(this->GetNormalCount() != N_vertex)
  {
    this->FillNormal();
  }
  ASSERT_CPE(this->GetNormalCount() == N_vertex, "Invalid normal computation");

  if(this->GetColorCount() != N_vertex)
  {
    this->FillColor({0.8f,0.8f,0.8f});
  }
  ASSERT_CPE(this->GetColorCount() == N_vertex, "Invalid color set up");

  if(this->GetTCoordCount() != N_vertex)
  {
    TCoords.clear();
    for(int k = 0; k < N_vertex; ++k)
    {
      this->AddTCoord({0.0f, 0.0f});
    }
  }
  ASSERT_CPE(this->GetTCoordCount() == N_vertex, "Invalid TCoord set up");

  if(this->GetMaterialIndexCount() != N_vertex)
  {
    this->FillMaterialIdx(-1);
  }
  ASSERT_CPE(this->GetMaterialIndexCount() == N_vertex, "Invalid MaterialIdx set up");

}

void gltkGeometry::ComputeAABB(cpe::vec3& corner_min, cpe::vec3& corner_max)
{
  int const N=this->GetVertexCount();
  if(N==0) return ;
  corner_min=this->GetVertex(0);
  corner_max=this->GetVertex(0);
  for(int k=1;k<N;++k)
  {
    cpe::vec3 const& p=this->GetVertex(k);
    for(int k_dim=0;k_dim<3;++k_dim)
    {
      if(p[k_dim]<corner_min[k_dim]) corner_min[k_dim]=p[k_dim];
      if(p[k_dim]>corner_max[k_dim]) corner_max[k_dim]=p[k_dim];
    }
  }
}
