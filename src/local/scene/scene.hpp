#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <QTime>

#include "mat3.hpp"
#include "vec3.hpp"
#include "mesh.hpp"
#include "mesh_opengl.hpp"
#include "camera_matrices.hpp"

#include <vector>


class myWidgetGL;

class scene
{
public:

    scene();

    /**  Method called only once at the beginning (load off files ...) */
    void load_scene();

    /**  Method called at every frame */
    void draw_scene();

    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);

private:

    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;

    /** Ground mesh */
    cpe::mesh mesh_ground;
    /** Ground mesh for OpenGL drawing */
    cpe::mesh_opengl mesh_ground_opengl;

    /** The id of the shader to draw meshes */
    GLuint shader_mesh;

    void setup_shader_mesh(GLuint shader_id);

};

#endif
