#ifndef GRID_HPP
#define GRID_HPP

#include "boost/serialization/access.hpp"
#include <cmath>
#include <random>
#include <utility>

#include "mesh.hpp"
#include "perlin.hpp"
#include "Tile.hpp"
#include "vec2.hpp"

class Grid
{
public:
  Grid();
  Grid(int key);

  void BuildGrid (cpe::vec2 origin);

  cpe::mesh GetMeshGrid() const;
  void SetMeshGrid(const cpe::mesh &value);

  int GetSquareSize() const;
  void SetSquareSize(int value);

  int GetKeyMap() const;
  void SetKeyMap(int value);

  Tile const& GetTile(unsigned int u, unsigned int v) const;
  Tile const& GetTile(float x, float y) const;

  cpe::vec2 const& GetOrigin() const;
  void SetOrigin(cpe::vec2 origin);

private:

  void BuildGrid ();

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
  std::vector<Tile> Tiles;

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
