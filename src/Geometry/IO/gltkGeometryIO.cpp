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

#include "gltkGeometryIO.hpp"
#include "ProjectConfig.h"

#include "error_handling.hpp"
#include "gltkGeometry.hpp"
#include "gltkGeometryReaderOBJ.hpp"
#include "gltkGeometryReaderOFF.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>
#include <cassert>

#include <map>

gltkGeometry LoadGeometry(std::string const& filename)
{
  std::string data_dir = DATA_DIR;
  data_dir.append( "/" );
  
  if(filename.find(".obj") != std::string::npos || filename.find(".OBJ") != std::string::npos)
  {
    return LoadOBJFromFile( data_dir + filename );
  }

  if(filename.find(".off") != std::string::npos || filename.find(".OFF") != std::string::npos)
  {
    return LoadOFFFromFile( data_dir + filename );
  }

  throw cpe::exception_cpe("Unknown extension for mesh file "+filename,EXCEPTION_PARAMETERS_CPE);
}
