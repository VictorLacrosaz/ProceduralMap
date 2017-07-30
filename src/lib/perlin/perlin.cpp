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

#include "perlin.hpp"

#include "error_handling.hpp"
#include "simplexnoise1234.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"


#include <cmath>

namespace cpe
{

perlin::perlin()
    :octave_data(5),persistency_data(0.3f),FrequencyStart(1.0),
      FrequencyStep(2.0)
{}
perlin::perlin(int octave_param,float persistency_param,float Start,float Step)
    :octave_data(octave_param),persistency_data(persistency_param),
      FrequencyStart(Start),FrequencyStep(Step)
{
    ASSERT_CPE(octave_param>0,"Octave should be >0");
    ASSERT_CPE(persistency_param>0,"Persistency should be > 0");
    ASSERT_CPE(persistency_param<1,"Persistency should be < 1");
    ASSERT_CPE(Start>0,"Frequency start should be > 0");
    ASSERT_CPE(Step>0,"Frequency step should be > 0");
}

float perlin::operator()(float const p) const
{
  float value = 0.0f;
  double frequency = FrequencyStart;
  double persistency = 1.0;
  //
  for(int k = 0;k < octave_data;k++)
    {
      value += persistency*(snoise1(p*frequency));
      frequency *= FrequencyStep;
      persistency *= persistency_data;
    }

  //
  float Normalisation = ((1-persistency_data)/(1-persistency));

  //
  return std::max(value*Normalisation,0.0f);
}

float perlin::operator()(vec2 const& p) const
{
  float value = 0.0f;
  double frequency = FrequencyStart;
  double persistency = 1.0;

  for(int k = 0;k < octave_data;k++)
    {
      value += persistency*(snoise2(p.x()*frequency,
                                    p.y()*frequency));

      frequency *= FrequencyStep;

      persistency *= persistency_data;
    }
  float Normalisation = ((1-persistency_data)/(1-persistency));


  return std::max(value*Normalisation,0.0f);
}

float perlin::operator()(vec3 const& p) const
{
  float value = 0.0f;
  double frequency = FrequencyStart;
  double persistency = 1.0;

  for(int k = 0;k < octave_data;k++)
    {
      value += persistency*(snoise3(p.x()*frequency,
                                    p.y()*frequency,
                                    p.z()*frequency));
      frequency *= FrequencyStep;
      persistency *= persistency_data;
    }

  float Normalisation = ((1-persistency_data)/(1-persistency));
  return std::max(value*Normalisation,0.0f);
}

float perlin::operator()(vec4 const& p) const
{
  float value = 0.0f;
  double frequency = FrequencyStart;
  double persistency = 1.0;

  for(int k = 0;k < octave_data;k++)
    {
      value += persistency*(snoise4(p.x()*frequency,
                                    p.y()*frequency,
                                    p.z()*frequency,
                                    p.w()*frequency));
      frequency *= FrequencyStep;
      persistency *= persistency_data;
    }
  float Normalisation = ((1-persistency_data)/(1-persistency));

  return value*Normalisation;
}


}
