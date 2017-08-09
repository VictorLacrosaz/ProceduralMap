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

#ifndef INTERSECTION_HELPER_HPP
#define INTERSECTION_HELPER_HPP

#include "vec3.hpp"
#include <vector>


namespace gltkIntersection
{

/** \brief Compute the intersection between a sphere and a ray
    \return true if there is an intersection, false otherwise.
    The t-value (parameter along the line) at the intersection point
    is filled in the parameters intersection_0 and intersection_1.
    Ray_direction is supposed to be a vector of norm 1.
*/
bool RaySphere(cpe::vec3 const& sphere_center,
               float       sphere_radius,
               cpe::vec3 const& ray_center,
               cpe::vec3 const& ray_direction,
               float&      intersection_0,
               float&      intersection_1);

/** \brief Möller–Trumbore ray-triangle intersection algorithm.
  orig and dir defines the ray origin and direction. WARNING : dir should not
  be normalized.
  v0, v1, v2 defines the triangle points.
  \return float distance from the ray origin to the intersection or 0.0 if no
  intersection is found.
*/
float RayTriangle(const cpe::vec3& orig, const cpe::vec3& dir,
                  const cpe::vec3& v0, const cpe::vec3& v1, const cpe::vec3& v2);

}

#endif
