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

#ifndef GLTK_GRID_TILE_HPP
#define GLTK_GRID_TILE_HPP

#include "vec3.hpp"

#include <vector>

/** \brief
 * Container class for a square tile
 */
class gltkGridTile
{
public:

  /** Constructor setting the tile ID */
  gltkGridTile();

  /** Get/Set the tile points.
   * 1st Triangle : 0 2 3 -- 2nd Triangle : 0 3 1
   */
  void SetPoints(cpe::vec3 points[4]);
  const cpe::vec3* GetPoints() const;

private:
  // Triangle 1 : 0 2 3
  // Triangle 2 : 0 3 1
  cpe::vec3 Points[4];
};

#endif
