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

  HeightMap = gltkPerlinNoise(4,0.1f,0.15f,2.0f);

  TemperatureMap = gltkPerlinNoise(4,0.1f,0.01f,2.0f);

  MoistureMap = gltkPerlinNoise(4,0.1,0.02,2.0);

  OffsetXY = std::pair<int,int> (0,0);

  Radius = 8;

  TileSize = 200;
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
  gltkGeometry m;

  int u = 0,v = 0;
  int Size  = 2*Radius+1;
  float HeightMax = 2.5*TileSize;
  for (int i = Origin.x()-Radius*TileSize;i <= Origin.x()+Radius*TileSize;i += TileSize)
  {

    for (int j = Origin.y()-Radius*TileSize;j <= Origin.y()+Radius*TileSize;j += TileSize)
    {
      //Height informations for the square
      float Height = HeightMax*HeightMap(cpe::vec3(v - Radius + Origin.x()/TileSize,
                                                   u -Radius+ Origin.y()/TileSize,KeyMap));

      float HeightN1 = HeightMax*HeightMap(cpe::vec3(v -Radius + 1 + Origin.x()/TileSize,
                                                     u - Radius + Origin.y()/TileSize,KeyMap));

      float HeightN2 = HeightMax*HeightMap(cpe::vec3(v - Radius + Origin.x()/TileSize,
                                                     u - Radius + 1 + Origin.y()/TileSize,KeyMap));

      float HeightN3 = HeightMax*HeightMap(cpe::vec3(v - Radius + 1 + Origin.x()/TileSize,
                                                     u - Radius + 1 + Origin.y()/TileSize,KeyMap));

//                //Temperature informations for the square
//                float Temp = TemperatureMap(cpe::vec3(v+Origin.x(),u+Origin.y(),KeyMap));
//                float TempN1 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+Origin.y(),KeyMap));
//                float TempN2 = TemperatureMap(cpe::vec3(v+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));
//                float TempN3 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));

//                //Moisture informations for the square
//                float Moisture = MoistureMap(cpe::vec3(v+Origin.x(),u+Origin.y(),KeyMap));
//                float MoistureN1 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+Origin.y(),KeyMap));
//                float MoistureN2 = MoistureMap(cpe::vec3(v+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));
//                float MoistureN3 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Origin.x(),u+std::pow(-1,u)+Origin.y(),KeyMap));

      m.AddVertex(cpe::vec3(i,Height,j));
      m.AddTCoord(cpe::vec2(u,v));



      if(Height > 0.7*HeightMax || HeightN1 > 0.7*HeightMax || HeightN2 > 0.7*HeightMax || HeightN3 > 0.7*HeightMax)
      {
        m.AddMaterialIndex(0);
      }
      else if(Height > 0.5*HeightMax || HeightN1 > 0.5*HeightMax || HeightN2 > 0.5*HeightMax || HeightN3 > 0.5*HeightMax)
      {
        m.AddMaterialIndex(1);
      }
      else if(Height > 0.3*HeightMax || HeightN1 > 0.3*HeightMax || HeightN2 > 0.3*HeightMax || HeightN3 > 0.3*HeightMax)
      {
        m.AddMaterialIndex(2);
      }
      else
      {
        m.AddMaterialIndex(3);
      }

//      if(std::pow(i-Origin.x(),2)+std::pow(j-Origin.y(),2) <= std::pow(Radius*TileSize,2))
//       {
      if(u < Size-1 && v < Size-1)
      {
        //Add triangle index
        m.AddTriangleIndex(cpe::triangle_index(u+v*Size,(u+1)+v*Size,u+1+(v+1)*Size));
        m.AddTriangleIndex(cpe::triangle_index(u+v*Size,u+1+(v+1)*Size,u+(v+1)*Size));

        //Push back new tile
        gltkGridTile tile = gltkGridTile();
        cpe::vec3 pts[4] = {
          cpe::vec3(i,Height,j), cpe::vec3(i+TileSize,HeightN1,j),
          cpe::vec3(i,HeightN2,j+TileSize),cpe::vec3(i+TileSize,HeightN3,j+TileSize)};
        tile.SetPoints(pts);
        Tiles.push_back(tile);
      }
//        }
      u++;
    }

    u = 0;
    v++;
  }

  m.FillNormal();
  m.FillEmptyFields();

  gltkMaterial grassMaterial = gltkMaterial("Grass",
    cpe::vec3(0.0, 0.0, 0.0), cpe::vec3(0.0, 0.0, 0.0), "Grass.jpg");
  gltkMaterial cropsMaterial = gltkMaterial("Crops",
    cpe::vec3(0.0, 0.0, 0.0), cpe::vec3(0.0, 0.0, 0.0), "champ.jpg");
  gltkMaterial argileMaterial = gltkMaterial("Argile",
    cpe::vec3(0.0, 0.0, 0.0), cpe::vec3(0.0, 0.0, 0.0), "Argile.jpeg");
  gltkMaterial rockMaterial = gltkMaterial("Rock",
    cpe::vec3(0.0, 0.0, 0.0), cpe::vec3(0.0, 0.0, 0.0), "rock.jpg");
  m.SetMaterials({rockMaterial, argileMaterial, cropsMaterial, grassMaterial});

  Geometry = m;
}

void gltkProceduralGrid::SetRadius(int value)
{
  Radius = value;
}

int gltkProceduralGrid::GetRadius() const
{
  return Radius;
}

cpe::vec2 const& gltkProceduralGrid::GetOrigin() const
{
  return Origin;
}
void gltkProceduralGrid::SetOrigin(cpe::vec2 origin)
{
  this->Origin = origin;
}

gltkGeometry gltkProceduralGrid::GetGeometry() const
{
  return Geometry;
}

void gltkProceduralGrid::SetGeometry(gltkGeometry const& value)
{
  Geometry = value;
}

int gltkProceduralGrid::GetTileSize() const
{
  return TileSize;
}

void gltkProceduralGrid::SetTileSize(int value)
{
  TileSize = value;
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

gltkGridTile const& gltkProceduralGrid::GetTile(float x, float z) const
{
  int i = std::floor(x/TileSize)-Origin.x()/TileSize;
  int j = std::floor(z/TileSize)-Origin.y()/TileSize;

  unsigned int u = i + Radius;
  unsigned int v = j + Radius;

  return GetTile(u, v);
}
