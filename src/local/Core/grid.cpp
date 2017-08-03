#include "grid.hpp"
#include "iostream"

Grid::Grid()
{
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dist (0, 99999999999);
  KeyMap = dist(gen);

  HeightMap = cpe::perlin(4,0.1f,0.05f,2.0f);

  TemperatureMap = cpe::perlin(4,0.1f,0.01f,2.0f);

  MoistureMap = cpe::perlin(4,0.1,0.02,2.0);

  OffsetXY = std::pair<int,int> (0,0);

  Radius = 10;

  SquareSize = 10;


}

void Grid::BuildGrid (cpe::vec2 Center)
{
  cpe::mesh m;

  int u = 0,v = 0;
  int Size  = 2*Radius+1;
  int HeightMax = 8*SquareSize;
  for (int i = Center.x()-Radius*SquareSize;i <= Center.x()+Radius*SquareSize;i += SquareSize)
    {

      for (int j = Center.y()-Radius*SquareSize;j <= Center.y()+Radius*SquareSize;j += SquareSize)
        {
          //Height informations for the square
//          float Height = HeightMax*HeightMap(cpe::vec3(v+Center.x(),u+Center.y(),KeyMap));
//          float HeightN1 = HeightMax*HeightMap(cpe::vec3(v+1+Center.x(),u+Center.y(),KeyMap));
//          float HeightN2 = HeightMax*HeightMap(cpe::vec3(v+Center.x(),u+1+Center.y(),KeyMap));
//          float HeightN3 = HeightMax*HeightMap(cpe::vec3(v+1+Center.x(),u+1+Center.y(),KeyMap));
          float Height = HeightMax*HeightMap(cpe::vec3(v+Center.x()/SquareSize,u+Center.y()/SquareSize,KeyMap));
          float HeightN1 = HeightMax*HeightMap(cpe::vec3(v+1+Center.x()/SquareSize,u+Center.y()/SquareSize,KeyMap));
          float HeightN2 = HeightMax*HeightMap(cpe::vec3(v+Center.x()/SquareSize,u+1+Center.y()/SquareSize,KeyMap));
          float HeightN3 = HeightMax*HeightMap(cpe::vec3(v+1+Center.x()/SquareSize,u+1+Center.y()/SquareSize,KeyMap));
//          //Temperature informations for the square
//          float Temp = TemperatureMap(cpe::vec3(v+Center.x(),u+Center.y(),KeyMap));
//          float TempN1 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Center.x(),u+Center.y(),KeyMap));
//          float TempN2 = TemperatureMap(cpe::vec3(v+Center.x(),u+std::pow(-1,u)+Center.y(),KeyMap));
//          float TempN3 = TemperatureMap(cpe::vec3(v+std::pow(-1,v)+Center.x(),u+std::pow(-1,u)+Center.y(),KeyMap));

//          //Moisture informations for the square
//          float Moisture = MoistureMap(cpe::vec3(v+Center.x(),u+Center.y(),KeyMap));
//          float MoistureN1 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Center.x(),u+Center.y(),KeyMap));
//          float MoistureN2 = MoistureMap(cpe::vec3(v+Center.x(),u+std::pow(-1,u)+Center.y(),KeyMap));
//          float MoistureN3 = MoistureMap(cpe::vec3(v+std::pow(-1,v)+Center.x(),u+std::pow(-1,u)+Center.y(),KeyMap));

          m.add_vertex(cpe::vec3(i,Height,j));
          m.add_texture_coord(cpe::vec2(u,v));



          if(Height > 0.7*HeightMax || HeightN1 > 0.7*HeightMax || HeightN2 > 0.7*HeightMax || HeightN3 > 0.7*HeightMax)
            {
              m.add_color(cpe::vec3(0,0,0));
            }
          else if(Height > 0.5*HeightMax || HeightN1 > 0.5*HeightMax || HeightN2 > 0.5*HeightMax || HeightN3 > 0.5*HeightMax)
            {
              m.add_color(cpe::vec3(0.25,0,0));
            }
          else if(Height > 0.3*HeightMax || HeightN1 > 0.3*HeightMax || HeightN2 > 0.3*HeightMax || HeightN3 > 0.3*HeightMax)
            {
              m.add_color(cpe::vec3(0.5,0,0));
            }
          else
            {
              m.add_color(cpe::vec3(0.75,0,0));
            }



          //if(std::pow(i-Center.x(),2)+std::pow(j-Center.y(),2) <= std::pow(Radius*SquareSize,2))
          // {
              if(u < Size-1 && v < Size-1)
                {


                  m.add_triangle_index({u+v*Size,u+(v+1)*Size,(u+1)+v*Size});
                  m.add_triangle_index({u+(v+1)*Size,u+1+(v+1)*Size,(u+1)+v*Size});


                }
          //  }
          u++;
        }

      u = 0;
      v++;
    }

  m.fill_normal();
  m.fill_empty_field_by_default();


  MeshGrid = m;
}
cpe::mesh Grid::getMeshGrid() const
{
  return MeshGrid;
}

void Grid::setMeshGrid(const cpe::mesh &value)
{
  MeshGrid = value;
}
int Grid::getSquareSize() const
{
  return SquareSize;
}

void Grid::setSquareSize(int value)
{
  SquareSize = value;
}

