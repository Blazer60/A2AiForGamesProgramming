/**
 * @file LoaderCommon.cpp
 * @author Ryan Purse
 * @date 03/01/2022
 */


#include "FileIoCommon.h"

#include <filesystem>
#include <fstream>
#include <string_view>

std::vector<std::string> fileSystem::ls(std::string_view path)
{
    std::vector<std::string> out;
    for (const auto &entry : std::filesystem::directory_iterator(path.data()))
    {
        if (isValidPath(entry.path().string()))
            out.emplace_back(entry.path().filename().string());
    }
    
    return out;
}

bool fileSystem::isValidPath(std::string_view path)
{
    return std::filesystem::exists(path.data());
}

std::vector<std::string> fileSystem::splitArgs(std::string_view args, char delim)
{
    std::vector<std::string> argVec;
    
    auto start = std::begin(args);
    auto end = std::find(start, std::end(args), delim);
    while (end != std::end(args))
    {
        argVec.emplace_back(start, end);
        start = end + 1;  // Skip over th delim
        end = std::find(start, std::end(args), delim);
    }
    argVec.emplace_back(start, std::end(args));  // Always add the last element as a single arg.
    return argVec;
}

void fileSystem::convertFile(std::string_view path, const fileSystem::loadMap &functionMap)
{
    std::ifstream fileStream(path.data());
    if (fileStream.bad())
    {
        debug::log("Path could not be found to file.", debug::severity::Minor);
        return;
    }
    
    // Perform the actual conversion per line (Only one function on each line).
    std::string line;
    while (std::getline(fileStream, line))
    {
        if (line.empty())
            continue;  // Do nothing if there is a blank line.
            
        // Find where the "function" name ends and where the args start.
        auto separator = std::find(line.begin(), line.end(), ' ');
        
        // Split the function name and args into separate views.
        std::string functionName(line.begin(), separator);
        std::string args(separator + 1, line.end());
        
        // Find the function name within the map.
        auto it = functionMap.find(functionName);
        
        if (it != functionMap.end())
            it->second(args); // Process the function.
        else
            debug::log("Function (" + functionName + ") could not be found in functionMap",debug::severity::Minor);
        
    }
}

void fileSystem::saveTestData(std::string_view path, const fileSystem::TestLog &testLog)
{
    std::ofstream fileStream(path.data());
    if (fileStream.bad() || fileStream.fail())
    {
        debug::log("Test data could not be saved to a file.", debug::severity::Minor);
        return;
    }
    
    for (int i = 0; i < testLog.aiTimes.size(); ++i)
    {
        fileStream  << testLog.aStarPathSize[i] << ' '
                    << testLog.aStarTimes[i]    << ' '
                    << testLog.aiPathSize[i]    << ' '
                    << testLog.aiTimes[i]       << "\n";
    }
    
    fileStream.close();
    debug::log("Test Data has been written to: " + std::string(path));
}
