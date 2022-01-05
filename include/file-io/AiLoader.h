/**
 * @file AiLoader.h
 * @author Ryan Purse
 * @date 03/01/2022
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "QlHelpers.h"

namespace fileSystem
{
    /**
     * @brief All of the information that is stored within an Ai file.
     */
    struct aiSymbols
    {
        std::string version;
        uint64_t episode;
        uint64_t episodeMax;
        uint64_t iterationMax;
        float rewardGoal;
        float rewardTowardGoal;
        float rewardAwayFromGoal;
        float rewardIntoWall;
        float explorationRate;
        float explorationRateMin;
        float alpha;  // Learning Rate
        float gamma;  // Discount Factor
        QTable qTable;
    };
    
    /**
     * @brief Loads an AI at the specified path.
     * @param path - A relative or fixed path.
     * @returns Information about the AI (this can be unpacked later).
     */
    [[nodiscard]] aiSymbols loadAi(std::string_view path);
}