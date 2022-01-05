/**
 * @file Grid.cpp
 * @author Ryan Purse
 * @date 01/12/2021
 */


#include "Grid.h"

#include <utility>

Grid::Grid(Grid::Cells cells, const int width)
    : mCells(std::move(cells)), mWidth(width)
{

}

glm::ivec2 Grid::indexToVector(Cell index) const
{
    if (0 > index || index >= mCells.size())
        return { -1, -1 };  // Not a valid cell, out of range.
    return { index % mWidth, static_cast<int>((index / mWidth)) };
}

Grid::Cell Grid::vectorToIndex(glm::ivec2 vector) const
{
    if (vector.x < 0 || vector.y < 0 || vector.x >= mWidth || vector.x * vector.y >= mCells.size())
        return -1;  // Not a valid space, out of range.
    return vector.x + vector.y * mWidth;
}

Grid::Cells Grid::getAdjacent(Grid::Cell cell) const
{
    Cells queriedCells { cell - mWidth, cell + mWidth };
    
    if ((cell % mWidth) != 0)  // Cell not along the left wall.
        queriedCells.emplace_back(cell - 1);
    
    if ((cell + 1) % mWidth != 0)  // Cell not along the right wall.
        queriedCells.emplace_back(cell + 1);
    
    return validateAllCells(queriedCells);
}

Grid::Cells Grid::getSurrounding(Grid::Cell cell) const
{
    Cells queriedCells { cell - mWidth, cell + mWidth };
    
    if ((cell % mWidth) != 0)  // Cell not along the left wall.
    {
        queriedCells.emplace_back(cell - 1);
        queriedCells.emplace_back(cell - 1 - mWidth);
        queriedCells.emplace_back(cell - 1 + mWidth);
    }
    
    if ((cell + 1) % mWidth != 0)  // Cell not along the right wall.
    {
        queriedCells.emplace_back(cell + 1);
        queriedCells.emplace_back(cell + 1 - mWidth);
        queriedCells.emplace_back(cell + 1 + mWidth);
    }
    
    return validateAllCells(queriedCells);
}

float Grid::getDistance(Grid::Cell a, Grid::Cell b) const
{
    return glm::distance(static_cast<glm::vec2>(indexToVector(a)), static_cast<glm::vec2>(indexToVector(b)));
}

Grid::Cell Grid::getOrthogonalDistance(Grid::Cell a, Grid::Cell b) const
{
    auto aVec = static_cast<glm::ivec2>(indexToVector(a));
    auto bVec = static_cast<glm::ivec2>(indexToVector(b));
    return getOrthogonalDistance(aVec, bVec);
}

Grid::Cell Grid::getOrthogonalDistance(const glm::ivec2 &a, const glm::ivec2 &b)
{
    auto diffVec = glm::abs(a - b);
    return diffVec.x + diffVec.y;
}

Grid::Cells Grid::getDistanceToOrthogonalWalls(Grid::Cell cell) const
{
    Cells out;
    const std::vector<glm::ivec2> directions { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
    const glm::ivec2 startPos = indexToVector(cell);
    
    for (const auto &direction : directions)
    {
        bool hitWall = false;
        auto currentCellPos = startPos;
        
        while (!hitWall)
        {
            currentCellPos += direction;
            hitWall = !verifyCell(currentCellPos);
        }
        
        const glm::ivec2 diff = currentCellPos - startPos;
        out.push_back(glm::abs(diff.x) + glm::abs(diff.y));
    }
    
    return out;
}

bool Grid::verifyCell(Grid::Cell cell) const
{
    const auto pos = indexToVector(cell);
    return isInBounds(cell, pos);
}

bool Grid::verifyCell(const glm::ivec2 &pos) const
{
    const Cell cell = vectorToIndex(pos);
    return isInBounds(cell, pos);
}

Grid::Cell Grid::moveToNextValidCell(Grid::Cell cell) const
{
    do
    {
        cell++;
        if (cell >= mCells.size())
            cell = 0;
    }
    while (!verifyCell(cell));
    return cell;
}

const Grid::Cells &Grid::getCells() const
{
    return mCells;
}

Grid::Cells Grid::validateAllCells(const Grid::Cells &cells) const
{
    Cells selected;
    for (const Cell &qCell : cells)
    {
        if (verifyCell(qCell))
            selected.emplace_back(qCell);  // Cell is in rage, therefore add it.
    }
    
    return selected;
}

bool Grid::isInBounds(Grid::Cell cell, const glm::ivec2 &pos) const
{
    if (cell < 0 || cell >= mCells.size())
        return false;
    if (pos.x < 0 || pos.x >= mWidth)
        return false;
    if (mCells[cell] == static_cast<Cell>(Grid::cellType::Wall))
        return false;
    
    return true;
}
