/**
 * @file GridMesh.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "Grid.h"
#include "RendererSystem.h"

/**
 * Renders a grid of cells.
 * @author Ryan Purse
 * @date 01/12/2021
 */
class GridMesh
{
public:
    /**
     * @param size - The size of the grid.
     */
    explicit GridMesh(const glm::ivec2 &size);
    
    /**
     * @brief Sets the colour of the specified grid position.
     * @param position - A Vector2. There is no bounds check.
     * @param colour - A Vector3 colour [0, 1].
     */
    void setCellColour(const glm::ivec2 &position, const glm::vec3 &colour);
    
    [[nodiscard]] const Mesh &getMesh() const;

protected:
    Mesh        mMesh;
    glm::vec2   mStartLocation  { 125.f, 125.f };
    glm::ivec2  mGridSize       {0 };
    glm::vec2   mCellSize       { 25.f, 25.f };  // How big the cells are for rendering.
};


