/**
 * @file GridMesh.cpp
 * @author Ryan Purse
 * @date 01/12/2021
 */


#include "GridMesh.h"

GridMesh::GridMesh(const glm::ivec2 &size)
    : mGridSize(size)
{
    const glm::vec3 white(1.f);
    
    const glm::vec2 topLeft     (-mCellSize.x / 2, -mCellSize.y / 2);
    const glm::vec2 topRight    ( mCellSize.x / 2, -mCellSize.y / 2);
    const glm::vec2 bottomLeft  (-mCellSize.x / 2,  mCellSize.y / 2);
    const glm::vec2 bottomRight ( mCellSize.x / 2,  mCellSize.y / 2);
    
    unsigned int indicesOffSet = 0;
    
    
    for (int j = 0; j < size.y; ++j)
    {
        for (int i = 0; i < size.x; ++i)
        {
            glm::vec2 offSet(static_cast<float>(i) * mCellSize.x + mStartLocation.x, static_cast<float>(j) * mCellSize.y + mStartLocation.y);
            mMesh.vertices.emplace_back(Vertex { offSet + bottomLeft, white });
            mMesh.vertices.emplace_back(Vertex { offSet + topLeft, white });
            mMesh.vertices.emplace_back(Vertex { offSet + topRight, white });
            mMesh.vertices.emplace_back(Vertex { offSet + bottomRight, white });
            
            mMesh.indices.emplace_back(2 + indicesOffSet);
            mMesh.indices.emplace_back(1 + indicesOffSet);
            mMesh.indices.emplace_back(0 + indicesOffSet);
            mMesh.indices.emplace_back(3 + indicesOffSet);
            mMesh.indices.emplace_back(2 + indicesOffSet);
            mMesh.indices.emplace_back(0 + indicesOffSet);
            indicesOffSet += 4;
        }
    }
}

void GridMesh::setCellColour(const glm::ivec2 &position, const glm::vec3 &colour)
{
    const int cellIndex = (position.x + position.y * mGridSize.x) * 4;
    mMesh.vertices[cellIndex + 0].colour = colour;
    mMesh.vertices[cellIndex + 1].colour = colour;
    mMesh.vertices[cellIndex + 2].colour = colour;
    mMesh.vertices[cellIndex + 3].colour = colour;
}

const Mesh &GridMesh::getMesh() const
{
    return mMesh;
}
