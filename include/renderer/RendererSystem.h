/**
 * @file RendererSystem.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "Shader.h"

#include <glfw3.h>

/**
 * The definition of a vertex used by the shader.
 * @author Ryan Purse
 * @date 01/12/2021
 */
struct Vertex
{
    glm::vec2 position;
    glm::vec3 colour;
    
    static int stride() { return sizeof(Vertex); }
};

/**
 * A mesh that can be passed into the render system.
 * @author Ryan Purse
 * @date 01/12/2021
 */
struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

/**
 * A simple system to render meshes in openGl.
 * @author Ryan Purse
 * @date 01/12/2021
 */
class RendererSystem
{
public:
    explicit RendererSystem(const glm::vec2 &resolution);
    
    /**
     * @brief Clears the screen back to the default colour.
     */
    static void clear() ;
    
    /**
     * @brief Draws a mesh to the screen. Equivalent to one draw call.
     * @param mesh - The mesh that you want drawn to the screen.
     */
    void draw(const Mesh &mesh);
    
protected:
    unsigned int mVertexBufferId {};
    unsigned int mVertexArrayId  {};
    unsigned int mIndexBufferId  {};
    
    Shader      mShader;
    glm::mat4   mProjectionMatrix;
    glm::mat4   mViewMatrix { glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)) };
};


