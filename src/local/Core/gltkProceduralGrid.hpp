#ifndef GLTK_PROCEDURAL_GRID_HPP
#define GLTK_PROCEDURAL_GRID_HPP

#include "boost/serialization/access.hpp"
#include <cmath>
#include <random>
#include <utility>

#include "gltkGridTile.hpp"
#include "mesh.hpp"
#include "perlin.hpp"
#include "vec2.hpp"

class gltkProceduralGrid
{
public:
  gltkProceduralGrid();
  gltkProceduralGrid(int key);

  void Build(cpe::vec2 origin);

  cpe::mesh GetMeshGrid() const;
  void SetMeshGrid(const cpe::mesh &value);

  int GetSquareSize() const;
  void SetSquareSize(int value);

  int GetKeyMap() const;
  void SetKeyMap(int value);

  gltkGridTile const& GetTile(unsigned int u, unsigned int v) const;
  gltkGridTile const& GetTile(float x, float y) const;

  cpe::vec2 const& GetOrigin() const;
  void SetOrigin(cpe::vec2 origin);

private:

  void Build();

  //Perlin noises for the creation of the map
  cpe::perlin HeightMap;
  cpe::perlin MoistureMap;
  cpe::perlin TemperatureMap;

  //Mouvement of the map from coordinate (0,0)
  std::pair<int,int> OffsetXY;

  //Radius of the circle grid
  int Radius;

  //Size of a square in the grid
  int SquareSize;

  //Mesh for openGL
  cpe::mesh MeshGrid;

  int KeyMap;

  //Map Tiles
  std::vector<gltkGridTile> Tiles;

  //Boost serialize function for data save
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      // Simply list all the fields to be serialized/deserialized.
      ar & KeyMap;

  }

  //Map origin
  cpe::vec2 Origin;
};

#endif // GRID_HPP
