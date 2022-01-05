/**
 * @file AiLoader.cpp
 * @author Ryan Purse
 * @date 03/01/2022
 */


#include "AiLoader.h"
#include "FileIoCommon.h"

#include <functional>

/**
 * @brief Inserts a Q-value into the Q-table
 * @param qTable - The QTable that you want to insert the value into
 * @param data - The data provided by a file.
 */
void insertQValue(QTable &qTable, std::string_view data)
{
    const auto splitData = fileSystem::splitArgs(data, ' ');
    ActionValues values;
    
    // Convert all the values to floats apart from the first one. The first one is the look-up value.
    for (int i = 0; i < values.size(); ++i)
        values[i] = std::stof(splitData[i + 1]);
    qTable.insert({ splitData[0], values });
}

fileSystem::aiSymbols fileSystem::loadAi(std::string_view path)
{
    aiSymbols info;
    
    // All the expected symbols at the start of every line followed by how they should interpret the expression.
    const fileSystem::loadMap functionMap {
            { "#ve", [&info](std::string_view data){ info.version               = std::string(data); } },
            { "#ep", [&info](std::string_view data){ info.episode               = std::stoull(data.data()); } },
            { "#em", [&info](std::string_view data){ info.episodeMax            = std::stoull(data.data()); } },
            { "#im", [&info](std::string_view data){ info.iterationMax          = std::stoull(data.data()); } },
            { "#xx", [&info](std::string_view data){ std::stof(data.data()); } },
            { "#rg", [&info](std::string_view data){ info.rewardGoal            = std::stof(data.data()); } },
            { "#rf", [&info](std::string_view data){ info.rewardTowardGoal      = std::stof(data.data()); } },
            { "#rb", [&info](std::string_view data){ info.rewardAwayFromGoal    = std::stof(data.data()); } },
            { "#rw", [&info](std::string_view data){ info.rewardIntoWall        = std::stof(data.data()); } },
            { "#xr", [&info](std::string_view data){ info.explorationRate       = std::stof(data.data()); } },
            { "#xm", [&info](std::string_view data){ info.explorationRateMin    = std::stof(data.data()); } },
            { "#al", [&info](std::string_view data){ info.alpha                 = std::stof(data.data()); } },
            { "#ga", [&info](std::string_view data){ info.gamma                 = std::stof(data.data()); } },
            { "#qt", [&info](std::string_view data){ insertQValue(info.qTable, data); } },
    };
    
    convertFile(path, functionMap);  // Actually start converting the file.
    
    return info;
}
