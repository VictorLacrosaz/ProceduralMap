#pragma once

#ifndef GLTK_RENDER_MANAGER_HPP
#define GLTK_RENDER_MANAGER_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "gltkCamera.hpp"
#include "gltkProceduralGrid.hpp"
#include "mesh.hpp"
#include "mesh_opengl.hpp"

#include <vector>



class gltkRenderManager
{
public:

   gltkRenderManager();

    /**  Method called only once at the beginning (load off files ...) */
    void Initialize();

    /**  Rendering callback */
    void Render();

    /**  Rendering callback */
    void SetTextures(std::vector<GLuint> t);

    //* Accesors */
    gltkCamera& GetCamera() {return Camera;}
    gltkCamera const& GetCamera() const {return Camera;}

    gltkProceduralGrid& GetGrid() {return ProceduralGrid;}
    gltkProceduralGrid const& GetGrid() const {return ProceduralGrid;}

private:

    /** Setup default shader for mesh rendering using default texture */
    void SetupShaders ();

    /** Vector of shaders ID */
    std::vector<GLuint> Shaders;

    /** Textures ID*/
    std::vector<GLuint> Textures;

    /** Ground mesh */
    cpe::mesh mesh_ground;
    /** Ground mesh for OpenGL drawing */
    cpe::mesh_opengl mesh_ground_opengl;

    /** Grid */
    gltkProceduralGrid ProceduralGrid;

    gltkCamera Camera;
};

#endif
