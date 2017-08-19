#pragma once

#ifndef GLTK_PROCEDURAL_GRID_HPP
#define GLTK_PROCEDURAL_GRID_HPP

#include "boost/serialization/access.hpp"
#include <cmath>
#include <random>
#include <utility>

#include "gltkGridTile.hpp"
#include "gltkPerlinNoise.hpp"
#include "gltkGeometry.hpp"
#include "vec2.hpp"

/** \brief
 * Procedural generation of a terrain tilemap using Perlin noise.
 */
class gltkProceduralGrid
{
public:
  gltkProceduralGrid();

  /** Generation of the map around the origin point */
  void Build(cpe::vec2 origin);

  /** Get/Set the origin */
  cpe::vec2 const& GetOrigin() const;
  void SetOrigin(cpe::vec2 origin);

  /** Get/Set the grid mesh */
  gltkGeometry GetGeometry() const;
  void SetGeometry(gltkGeometry const& value);

  /** Get/Set the tiles size */
  int GetTileSize() const;
  void SetTileSize(int value);

  /** Access a tile given a normalized grid position */
  gltkGridTile const& GetTile(unsigned int u, unsigned int v) const;
  /** Access a tile given a world XZ position */
  gltkGridTile const& GetTile(float x, float z) const;

   /** Get/Set the Key for the map generation */
  int GetKeyMap() const;
  void SetKeyMap(int value);

   /** Get/Set the grid radius */
  int GetRadius() const;
  void SetRadius(int value);

private:

  //Internal generation of the map
  void Build();

  //Perlin noises for the creation of the map
  gltkPerlinNoise HeightMap;
  gltkPerlinNoise MoistureMap;
  gltkPerlinNoise TemperatureMap;

  //Mouvement of the map from coordinate (0,0)
  std::pair<int,int> OffsetXY;

  //Radius of the circle grid
  int Radius;

  //Map origin
  cpe::vec2 Origin;

  //Size of a tile in the grid
  int TileSize;

  //Map Tiles
  std::vector<gltkGridTile> Tiles;

  //Mesh for openGL
  gltkGeometry Geometry;

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
