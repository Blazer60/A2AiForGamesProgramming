/**
 * @file QlPathFinder.h
 * @author Ryan Purse
 * @date 02/01/2022
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "Agent.h"
#include "Environment.h"

class Grid;

/**
 * Binds and controls how an environment and agent interact with each other.
 * Allows the agent to be trained in an environment and an agent to run in an environment.
 * @author Ryan Purse
 * @date 02/01/2022
 */
class QlPathFinder
{
public:
    /**
     * @brief Initialises QlPathFinder. Not a constructor since it can be called multiple times.
     * @param grid - The grid/maze that it will be acting on.
     */
    void init(const std::shared_ptr<Grid>& grid);
    
    /**
     * @brief Performs a single tick for the Ai.
     */
    void update();
    
    /**
     * @brief Performs a single tick for the Ai. Trains the Ai based on various parameters.
     */
    void updateTrain();
    
    /**
     * @brief Calculates a path between the start and end position. If the size of the vector
     * is >= maximum iterations, it has failed to find a path between the two points.
     * @param start - Where the start position is.
     * @param end - Where the finish is.
     * @returns A path between the two points.
     */
    [[nodiscard]] std::vector<int> calculatePath(const glm::ivec2 &start, const glm::ivec2 &end);
    
    /**
     * @brief Calculates a path between the start and end position. Additionally trains the Ai in the
     * attempt to make the next path more efficient. If the size of the vector is >= maximum iterations,
     * is has failed to find a path between the two points.
     * @param start - The start position.
     * @param end - The finish position.
     * @returns A path between the two points.
     */
    [[nodiscard]] std::vector<int> trainPath(const glm::ivec2 &start, const glm::ivec2 &end);
    
    /**
     * @brief Resets the Ai for a given start and end goal. Used in conjunction with update()
     * @param start - The start position.
     * @param end - The finish position.
     */
    void reset(const glm::ivec2 &start, const glm::ivec2 &end);
    
    /**
     * @brief Resets the ai for a given start and end gol. Used in conjunction with updateTrain().
     * @param start - the start position.
     * @param end - The finish position.
     */
    void resetTraining(const glm::ivec2 &start, const glm::ivec2 &end);
    
    /**
     * @brief Allows ImGui to show and configure the options for the Ai.
     */
    void renderOptions();
    
    /**
     * @brief Shows stats to ImGui that cannot be changed.
     */
    void renderTrainingStats();
    
    /**
     * @brief Checks to see if training the Ai is finished or not.
     * @returns True if it the training is finished, false otherwise.
     */
    [[nodiscard]] bool isTrainingFinished() const;
    
    /**
     * @brief Saves the Ai to a given path with the specified name.
     * @param path - A path to a valid folder that exist on disk.
     * @param name - The name that you want to give the Ai. (you need to include the extension).
     */
    void saveAi(std::string_view path, std::string_view name) const;
    
    /**
     * @brief Load an Ai from the given path.
     * @param path - A path to the ai file that you want to load.
     */
    void loadAi(std::string_view path);
    
protected:
    Agent       mAgent;
    Environment mEnvironment;
    uint64_t    mEpisode            { 0 };
    uint64_t    mEpisodeMax         { 10'000ull };
    uint64_t    mIteration          { 0 };
    uint64_t    mIterationMax       { 500ull };
    bool        mIsEpisodeFinished  { false };
    std::string mVersion            { "1.01" };
};


