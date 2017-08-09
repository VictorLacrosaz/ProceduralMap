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

#include "gltkIntersection.hpp"

#include <cmath>
#include <vector>


namespace gltkIntersection
{

//-------------------------------------------------------------------
// Shere - Ray intersection
//-------------------------------------------------------------------
bool RaySphere(cpe::vec3 const& sphere_center,
               float       sphere_radius,
               cpe::vec3 const& ray_center,
               cpe::vec3 const& ray_direction,
               float&      intersection_0,
               float&      intersection_1)
{
  cpe::vec3 const x0 = ray_center - sphere_center;
  float const dx = dot(ray_direction, x0);

  float const delta = dx*dx -dot(x0, x0) + sphere_radius*sphere_radius;

  if(delta < 0) //no intersection
  {
    return false;
  }
  else if(delta < 1e-6)
  {
    intersection_0 = -dx;
    intersection_1 = -dx;

    return true;
  }
  else
  {
    float const s = std::sqrt(delta);
    intersection_0 = -dx + s;
    intersection_1 = dx + s;

    return true;
  }
}

//-------------------------------------------------------------------
// Triangle - Ray intersection
//-------------------------------------------------------------------
float RayTriangle(const cpe::vec3& orig, const cpe::vec3& dir,
                  const cpe::vec3& v0, const cpe::vec3& v1, const cpe::vec3& v2)
{
  cpe::vec3 e1 = v1 - v0;
  cpe::vec3 e2 = v2 - v0;
  // Calculate planes normal vector
  cpe::vec3 pvec = cross(dir, e2);
  float det = dot(e1, pvec);

  // Ray is parallel to plane
  if (det < 1e-8 && det > -1e-8)
  {
    return 0;
  }

  float inv_det = 1 / det;
  cpe::vec3 tvec = orig - v0;
  float u = dot(tvec, pvec) * inv_det;
  if (u < 0 || u > 1)
  {
    return 0;
  }

  cpe::vec3 qvec = cross(tvec, e1);
  float v = dot(dir, qvec) * inv_det;
  if (v < 0 || u + v > 1)
  {
    return 0;
  }
  return dot(e2, qvec) * inv_det;
}

}//End namespace intersection
