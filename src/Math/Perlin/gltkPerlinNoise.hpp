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

#ifndef GLTK_PERLIN_NOISE_HPP
#define GLTK_PERLIN_NOISE_HPP

namespace cpe
{
class vec2;
class vec3;
class vec4;
}

class gltkPerlinNoise
{
public:

  /** Empty constructor with default values */
  gltkPerlinNoise();
  /** Provides octave and persistency parameters (persistency=1/attenuation)
     *
     * octave should be a number >0
     * persistency should be in ]0,1[
    */
  gltkPerlinNoise(int Octave,float PersistencyData,float Start,float Step);

  /** Perlin noise in 1D */
  float operator()(float p) const;
  /** Perlin noise in 2D */
  float operator()(cpe::vec2 const& p) const;
  /** Perlin noise in 3D */
  float operator()(cpe::vec3 const& p) const;
  /** Perlin noise in 4D */
  float operator()(cpe::vec4 const& p) const;

private:
  /** The number of octave (the number of sumation) */
  int OctaveNumber;
  /** The attenuation for each octave */
  float Persistency;
  /** The frequency for the first octave*/
  float FrequencyStart;
  /** The Frequency step between each ocyave*/
  float FrequencyStep;
};

#endif
