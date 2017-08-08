#include "gltkProceduralGrid.hpp"

#include "error_handling.hpp"

#include <iostream>
#include "limits.h"

gltkProceduralGrid::gltkProceduralGrid()
{
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dist (0, INT_MAX);
  KeyMap = dist(gen);

  HeightMap = cpe::perlin(4,0.1f,0.15f,2.0f);

  TemperatureMap = cpe::perlin(4,0.1f,0.01f,2.0f);

  MoistureMap = cpe::perlin(4,0.1,0.02,2.0);

  OffsetXY = std::pair<int,int> (0,0);

  Radius = 8;

  SquareSize = 200;
}

void gltkProceduralGrid::Build(cpe::vec2 origin)
{
  // Clear Tiles
  if (Tiles.size() > 0)
  {
    Tiles.clear();
  }
  this->Origin = origin;

  // Build the map
  this->Build();
}

void gltkProceduralGrid::Build()
{
  cpe::mesh m;

  int u = 0,v = 0;
  int Size  = 2*Radius+1;
  float HeightMax = 2.5*SquareSize;
  for (int i = Origin.x()-Radius*SquareSize;i <= Origin.x()+Radius*SquareSize;i += SquareSize)
    {

      for (int j = Origin.y()-Radius*SquareSize;j <= Origin.y()+Radius*SquareSize;j += SquareSize)
        {
          //Height informations for the square
          float Height = HeightMax*HeightMap(cpe::vec3(v+Origin.x()/SquareSize,u+Origin.y()/SquareSize,KeyMap));
          float HeightN1 = HeightMax*HeightMap(cpe::vec3(v+1+Origin.x()/SquareSize,u+Origin.y()/SquareSize,KeyMap));
          float HeightN2 = HeightMax*HeightMap(cpe::vec3(v+Origin.x()/SquareSize,u+1+Origin.y()/SquareSize,KeyMap));
          float HeightN3 = HeightMax*HeightMap(cpe::vec3(v+1+Origin.x()/SquareSize,u+1+Origin.y()/SquareSize,KeyMap));

//          //Temperature informations for the square
//          float Temp = TemperatureMap(cpe::vec3(v+Origin.x(),u+Origin.y(),KeyMap));
//          float TempN1 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+Origin.y(),KeyMap));
//          float TempN2 = TemperatureMap(cpe::vec3(v+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));
//          float TempN3 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));

//          //Moisture informations for the square
//          float Moisture = MoistureMap(cpe::vec3(v+Origin.x(),u+Origin.y(),KeyMap));
//          float MoistureN1 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+Origin.y(),KeyMap));
//          float MoistureN2 = MoistureMap(cpe::vec3(v+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));
//          float MoistureN3 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));

          m.add_vertex(cpe::vec3(i,Height,j));
          m.add_texture_coord(cpe::vec2(u,v));



          if(Height > 0.7*HeightMax || HeightN1 > 0.7*HeightMax || HeightN2 > 0.7*HeightMax || HeightN3 > 0.7*HeightMax)
            {
              m.add_color(cpe::vec3(0,0,0));
            }
          else if(Height > 0.5*HeightMax || HeightN1 > 0.5*HeightMax || HeightN2 > 0.5*HeightMax || HeightN3 > 0.5*HeightMax)
            {
              m.add_color(cpe::vec3(0.25,0,0));
            }
          else if(Height > 0.3*HeightMax || HeightN1 > 0.3*HeightMax || HeightN2 > 0.3*HeightMax || HeightN3 > 0.3*HeightMax)
            {
              m.add_color(cpe::vec3(0.5,0,0));
            }
          else
            {
              m.add_color(cpe::vec3(0.75,0,0));
            }
//std::cout<<"H : "<<Height<<std::endl;
//std::cout<<"H1 : "<<HeightN1<<std::endl;
//std::cout<<"H2 : "<<HeightN2<<std::endl;
//std::cout<<"H3 : "<<HeightN3<<std::endl;


          //if(std::pow(i-Origin.x(),2)+std::pow(j-Origin.y(),2) <= std::pow(Radius*SquareSize,2))
          // {
              if(u < Size-1 && v < Size-1)
                {


                  m.add_triangle_index({u+v*Size,(u+1)+v*Size,u+1+(v+1)*Size});
                  m.add_triangle_index({u+v*Size,u+1+(v+1)*Size,u+(v+1)*Size});

                  gltkGridTile tile = gltkGridTile();
                  cpe::vec3 pts[4] = {
                    cpe::vec3(i,Height,j), cpe::vec3(i+SquareSize,HeightN1,j),
                    cpe::vec3(i,HeightN2,j+SquareSize),cpe::vec3(i+SquareSize,HeightN3,j+SquareSize)};
                  tile.SetPoints(pts);
                  Tiles.push_back(tile);


                }
          //  }
          u++;
        }

      u = 0;
      v++;
    }

  m.fill_normal();
  m.fill_empty_field_by_default();

  MeshGrid = m;
}

cpe::vec2 const& gltkProceduralGrid::GetOrigin() const
{
  return Origin;
}
void gltkProceduralGrid::SetOrigin(cpe::vec2 origin)
{
  this->Origin = origin;
}

cpe::mesh gltkProceduralGrid::GetMeshGrid() const
{
  return MeshGrid;
}

void gltkProceduralGrid::SetMeshGrid(const cpe::mesh &value)
{
  MeshGrid = value;
}

int gltkProceduralGrid::GetSquareSize() const
{
  return SquareSize;
}

void gltkProceduralGrid::SetSquareSize(int value)
{
  SquareSize = value;
}

int gltkProceduralGrid::GetKeyMap() const
{
  return KeyMap;
}

void gltkProceduralGrid::SetKeyMap(int value)
{
  KeyMap = value;
}

gltkGridTile const& gltkProceduralGrid::GetTile(unsigned int u, unsigned int v) const
{
  ASSERT_CPE(u * (2 * Radius) + v < Tiles.size(),"Tile ID out of bounds");
  return Tiles[u * (2 * Radius) + v];
}

gltkGridTile   const& gltkProceduralGrid::GetTile(float x, float y) const
{
  int i = std::floor(x/SquareSize)-Origin.x()/SquareSize;
  int j = std::floor(y/SquareSize)-Origin.y()/SquareSize;

  unsigned int u = i + Radius;
  unsigned int v = j + Radius;

  return GetTile(u, v);
}
