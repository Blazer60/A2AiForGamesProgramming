/**
 * @file Grid.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

/**
 * Grid holds an array of integers with useful functions to find coords and get adjacent cells.
 * @author Ryan Purse
 * @date 01/12/2021
 */
class Grid
{
public:
    typedef int Cell;
    typedef std::vector<Cell> Cells;
    enum class cellType : Cell { Empty, Wall, Start, Finish, OpenSet, ClosedSet, Path };
    
public:
    /**
     * @param cells - The grid/maze of integers.
     * @param width - The width of the grid/maze
     */
    Grid(Cells cells, int width);
    
    ~Grid() = default;
    
    /**
     * @brief Gets the 2D coordinates of a grid cell based on the index of the cell.
     * @param index - The index that you want to transform into a 2D Vector.
     * @returns 2D Coordinates of the grid cell.
     */
    [[nodiscard]] glm::ivec2 indexToVector(Cell index) const;
    
    /**
     * @brief Transforms a 2D vector into a cell index. There is no bounds checking.
     * @param vector - The vector you want to turn into an index.
     * @returns an index based on the vector provided.
     */
    [[nodiscard]] Cell vectorToIndex(glm::ivec2 vector) const;
    
    /**
     * @brief Gets all the cell orthogonal to the cell provided.
     * @param cell - The index to query.
     * @returns All cells adjacent to the original cell.
     */
    [[nodiscard]] Cells getAdjacent(Cell cell) const;
    
    /**
     * @brief Gets all of the Cells touching this cell including diagonals.
     * @param cell - The index to query.
     * @returns All cells that surround the original cell.
     */
    [[nodiscard]] Cells getSurrounding(Cell cell) const;
    
    /**
     * @brief Gets the Pythagorean distance between a and b.
     * @param a - The Start Cell
     * @param b - The End Cell
     * @returns The distance between the two cells.
     */
    [[nodiscard]] float getDistance(Cell a, Cell b) const;
    
    /**
     * @brief Gets the summed difference between the two axis. (∆x + ∆y)
     * @param a - The start Cell.
     * @param b - The end Cell.
     * @returns (∆x + ∆y)
     */
    [[nodiscard]] Cell getOrthogonalDistance(Cell a, Cell b) const;
    
    /**
     * @brief Gets the summed difference between the two axis. (∆x + ∆y)
     * @param a - The start Cell.
     * @param b - The end Cell.
     * @returns (∆x + ∆y)
     */
    [[nodiscard]] static Cell getOrthogonalDistance(const glm::ivec2 &a, const glm::ivec2 &b);
    
    /**
     * @brief Gets the distance to the walls in [x, -x, y, -y] directions.
     * @param cell - The cell that you want to expand from.
     * @returns The distances to each wall [north, east, south, west].
     */
    [[nodiscard]] Cells getDistanceToOrthogonalWalls(Cell cell) const;
    
    /**
     * @brief Checks to see if the cell is out of bounds or of type wall.
     * @param cell - The cell that you want to check.
     * @returns True if a valid cell and not a wall, false otherwise.
     */
    [[nodiscard]] bool verifyCell(Cell cell) const;
    
    /**
     * @brief Checks to see if the cell is out of bounds or of type wall.
     * @param pos - The positions that you want to check.
     * @returns True if a valid position and not a wall, false otherwise.
     */
    [[nodiscard]] bool verifyCell(const glm::ivec2 &pos) const;
    
    /**
     * @brief Returns the next valid cell along in the grid/maze (loops down and back to the top).
     * @param cell - The current cell.
     * @returns The next valid cell.
     */
    [[nodiscard]] Cell moveToNextValidCell(Cell cell) const;
    
    /**
     * @returns Returns a const ref to the underlying cells.
     */
    [[nodiscard]] const Cells &getCells() const;
    
protected:
    Cells mCells;
    int mWidth;
    
    /**
     * @brief Validates all the cells in a list and returns a new list of valid cells.
     * @param cells - The Cells that you want to validate.
     * @returns A valid list of cells.
     */
    [[nodiscard]] Cells validateAllCells(const Cells &cells) const;
    
    /**
     * @brief Checks to see if cell is in bounds. Call verifyCell() first.
     * @see bool verifyCell(*);
     */
    [[nodiscard]] bool isInBounds(Cell cell, const glm::ivec2 &pos) const;
};


