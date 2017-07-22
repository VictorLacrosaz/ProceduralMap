#include "scene.hpp"

#include <GL/glew.h>

#include "camera_matrices.hpp"
#include "glutils.hpp"
#include "mesh_io.hpp"
#include "myWidgetGL.hpp"
#include "perlin.hpp"

#include <cmath>
#include <string>
#include <sstream>


using namespace cpe;

static cpe::mesh BuildGrid (vec2 Center,int Radius)
{
     mesh m;
     int u = 0,v = 0;
     int Size  = 2*Radius+1;
     for (int i = Center.x()-Radius;i <= Center.x()+Radius;i++)
     {
       for (int j = Center.y()-Radius;j <= Center.y()+Radius;j++)
       {
           m.add_vertex(vec3(i,j,0));
           if(std::pow(i,2)+std::pow(j,2) < std::pow(Radius,2))
           {
               if(u < Size-1 && v < Size-1)
               {

                   m.add_triangle_index({u+v*Size,u+(v+1)*Size,(u+1)+v*Size});
                   m.add_triangle_index({u+(v+1)*Size,u+1+(v+1)*Size,(u+1)+v*Size});
               }
           }
           u++;
       }

       u = 0;
       v++;
     }

     m.fill_color(vec3(0.8,0.6,0.6));

    return m;
}


static cpe::mesh build_ground(float const L,float const h)
{
    mesh m;
    m.add_vertex(vec3(-L, h,-L));
    m.add_vertex(vec3(-L, h, L));
    m.add_vertex(vec3( L, h, L));
    m.add_vertex(vec3( L, h,-L));

    m.add_triangle_index({0,2,1});
    m.add_triangle_index({0,3,2});

    m.fill_color(vec3(0.8,0.9,0.8));

    return m;
}


void scene::load_scene()
{
    //*****************************************//
    // Preload default structure               //
    //*****************************************//
    texture_default = load_texture_file("white.jpg");

    shader_mesh     = read_shader("shader_mesh.vert",
                                  "shader_mesh.frag");           PRINT_OPENGL_ERROR();

    //*****************************************//
    // Build ground
    //*****************************************//
    mesh_ground = BuildGrid(vec2(0,0) ,200);
    mesh_ground.fill_empty_field_by_default();
    mesh_ground_opengl.fill_vbo(mesh_ground);
}

void scene::draw_scene()
{
    // Draw the ground
    setup_shader_mesh(shader_mesh);
    mesh_ground_opengl.draw();
}


void scene::setup_shader_mesh(GLuint const shader_id)
{
    //Setup uniform parameters
    glUseProgram(shader_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();

    //load white texture
    glBindTexture(GL_TEXTURE_2D,texture_default);                                                      PRINT_OPENGL_ERROR();
    glLineWidth(1.0f);                                                                                 PRINT_OPENGL_ERROR();

}

scene::scene()
    :shader_mesh(0)
{}

GLuint scene::load_texture_file(std::string const& filename)
{
  std::string data_dir = DATA_DIR;
  data_dir.append( "/" );
  return pwidget->load_texture_file( data_dir + filename );
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}


