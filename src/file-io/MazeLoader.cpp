/**
 * @file MazeLoader.cpp
 * @author Ryan Purse
 * @date 02/12/2021
 */


#include "MazeLoader.h"

#include "FileIoCommon.h"

#include <fstream>

fileSystem::MazeData processMaze(std::ifstream &fileStream)
{
    fileSystem::MazeData grid;
    bool hasWidth = false;
    bool hasHeight = false;
    
    std::string line;
    while (std::getline(fileStream, line))
    {
        if (line.empty())  // Ignore that line if it's empty.
            continue;
        
        // Split args by whitespace as multiple pieces of information could be on a single line.
        for (const auto &arg : fileSystem::splitArgs(line))
        {
            int num = std::stoi(arg);
            
            // The first number encountered is always the width.
            if (!hasWidth)
            {
                grid.gridSize.x = num;
                hasWidth = true;
                continue;
            }
            
            // The second number encountered is always the height.
            if (!hasHeight)
            {
                grid.gridSize.y = num;
                hasHeight = true;
                continue;
            }
            
            switch (num)
            {
                default:  // Empty
                    grid.grid.emplace_back(0);
                    break;
                case 1:  // Wall
                    grid.grid.emplace_back(1);
                    break;
                case 2:  // Start
                    grid.startIndex = static_cast<int>(grid.grid.size());
                    grid.grid.emplace_back(0);
                    break;
                case 3:  // End
                    grid.endIndex = static_cast<int>(grid.grid.size());
                    grid.grid.emplace_back(0);
                    break;
            }
        }
    }
    
    return grid;
}

fileSystem::MazeData fileSystem::loadMaze(std::string_view path)
{
    std::ifstream fileStream(path.data());
    if (fileStream.bad() || fileStream.fail())
    {
        debug::log("The file does not exist:\n" + std::string(path) + "\nCheck the file path.", debug::severity::Minor);
        return { };
    }
    
    try
    {
        return processMaze(fileStream);
    }
    catch (const std::exception &e)
    {
        debug::log("The file could not be processed: " + std::string(e.what()), debug::severity::Minor);
    }
    return { };
}
