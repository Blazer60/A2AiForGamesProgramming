/**
 * @file RendererSystem.cpp
 * @author Ryan Purse
 * @date 01/12/2021
 */


#include "RendererSystem.h"

RendererSystem::RendererSystem(const glm::vec2 &resolution)
    : mProjectionMatrix(glm::ortho(0.f, resolution.x, resolution.y, 0.f, -1.f, 1.f)),
      mShader("../res/shaders/Grid.shader")
{
    glGenBuffers(1, &mVertexBufferId);
    glGenBuffers(1, &mIndexBufferId);
    glGenVertexArrays(1, &mVertexArrayId);
    
    
    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);  // Binding a buffer does nto attach it to a VAO.
    
    glEnableVertexAttribArray(0);  // Position
    glEnableVertexAttribArray(1);  // Colour
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(offsetof(Vertex, colour)));
    
    mShader.bind();
    
    glClearColor(0.16f, 0.16f, 0.16f, 1.f);
}

void RendererSystem::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererSystem::draw(const Mesh &mesh)
{
    mShader.bind();
    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
    
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * Vertex::stride(), &mesh.vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_DYNAMIC_DRAW);
    
    // Set uniforms for shader. Can possibly be done in setup.
    mShader.setUniform("u_mvp_matrix", mProjectionMatrix * mViewMatrix);
    
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}
