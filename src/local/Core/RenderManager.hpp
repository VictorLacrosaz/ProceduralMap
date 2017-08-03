#pragma once

#ifndef RENDERMANAGER_HPP
#define RENDERMANAGER_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "Camera.hpp"
#include "mesh.hpp"
#include "mesh_opengl.hpp"
#include "grid.hpp"

#include <vector>



class RenderManager
{
public:

    RenderManager();

    /**  Method called only once at the beginning (load off files ...) */
    void Initialize();

    /**  Rendering callback */
    void Render();

    /**  Rendering callback */
    void SetTextures(std::vector<GLuint> t);

    //* Accesors */
    Camera& GetCamera() {return MainCamera;}
    Camera const& GetCamera() const {return MainCamera;}


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
    Grid GameGrid;

    Camera MainCamera;
};

#endif
