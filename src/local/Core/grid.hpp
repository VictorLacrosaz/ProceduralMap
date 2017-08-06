#ifndef GRID_HPP
#define GRID_HPP

#include "boost/serialization/access.hpp"
#include "cmath"
#include "random"

#include "mesh.hpp"
#include "perlin.hpp"
#include "utility"
#include "vec2.hpp"

class Grid
{
public:
  Grid();
  Grid(int key);

  void BuildGrid (cpe::vec2 Center);

  cpe::mesh GetMeshGrid() const;
  void SetMeshGrid(const cpe::mesh &value);

  int GetSquareSize() const;
  void SetSquareSize(int value);

  int GetKeyMap() const;
  void SetKeyMap(int value);

private:

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


  //Boost serialize function for data save
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      // Simply list all the fields to be serialized/deserialized.
      ar & KeyMap;

  }

};

#endif // GRID_HPP
