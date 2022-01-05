/**
 * @file LoaderCommon.h
 * @author Ryan Purse
 * @date 03/01/2022
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include <functional>

namespace fileSystem
{
    typedef std::unordered_map<std::string_view, std::function<void(std::string_view)>> loadMap;
    
    /**
     * @brief The recorded information when testing the Ai Vs. A*.
     */
    struct TestLog
    {
        std::vector<long long> aStarTimes;
        std::vector<long long> aiTimes;
        std::vector<uint64_t> aStarPathSize;
        std::vector<uint64_t> aiPathSize;
    };
    
    /**
     * @brief Lists all items within a given directory.
     * @param path - The path to a directory.
     * @returns All items in the given path.
     */
    [[nodiscard]] std::vector<std::string> ls(std::string_view path);
    
    /**
     * @brief Validates a path to see if it exists.
     * @param path - The path that you want to validate.
     * @returns True if valid, false otherwise.
     */
    [[nodiscard]] bool isValidPath(std::string_view path);
    
    /**
     * @brief Splits a string based on the deliminator provided.
     * @param args - The string you want to split.
     * @param delim - The deliminator used. defaults to ' '.
     * @returns A vector of strings split by delim.
     */
    [[nodiscard]] std::vector<std::string> splitArgs(std::string_view args, char delim=' ');
    
    /**
     * @brief Converts a file based on the given function map.
     * @param path - The path to the file you want to convert.
     * @param functionMap - A function map (tokens -> functions). E.g.: "#an" -> adds a number to a variable.
     */
    void convertFile(std::string_view path, const loadMap &functionMap);
    
    /**
     * @brief Saves a test log to a given file path.
     * @param path - The path that you want to save it to.
     * @param testLog - The information that you want to save.
     */
    void saveTestData(std::string_view path, const fileSystem::TestLog &testLog);
}


