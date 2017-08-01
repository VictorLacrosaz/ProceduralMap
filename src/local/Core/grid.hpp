#ifndef GRID_HPP
#define GRID_HPP

#include "perlin.hpp"
#include "utility"
#include "mesh.hpp"
#include "vec2.hpp"
#include "cmath"
#include "random"



class Grid
{
public:
  Grid();

  void BuildGrid (cpe::vec2 Center);

  cpe::mesh getMeshGrid() const;
  void setMeshGrid(const cpe::mesh &value);

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






};

#endif // GRID_HPP
