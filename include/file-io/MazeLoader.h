/**
 * @file MazeLoader.h
 * @author Ryan Purse
 * @date 02/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

namespace fileSystem
{
    /**
     * @brief The data that the maze file creates.
     */
    struct MazeData
    {
        std::vector<int> grid;
        glm::ivec2 gridSize { 0 };
        int startIndex { 0 };
        int endIndex { 0 };
    };

    /**
     * @brief Creates MazeData based on the path provided.
     * @param path - The file path of the maze.
     * @returns The maze data from the file.
     */
    [[nodiscard]] MazeData loadMaze(std::string_view path);
}



