/**
 * @file Environment.h
 * @author Ryan Purse
 * @date 31/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "QlHelpers.h"
#include "Agent.h"
#include "AiLoader.h"

class Grid;

struct Rewards
{
    float goal          { 1.f };
    float towardsGoal   { 0.02f };
    float awayFromGoal  { -0.01f };
    float intoWall      { -0.1f };
};

/**
 * A Q-Learning environment so that an agent can learn in a grid based environment.
 * @author Ryan Purse
 * @date 31/12/2021
 */
class Environment
{
public:
    /** @brief The current state that was processed by the environment. */
    State                   state;
    
    /** @brief The 'real' environment that the virtual environment is working on. */
    std::shared_ptr<Grid>   grid;
    
    /** @brief The position of the goal */
    glm::ivec2              goal { 0 };
    
    /**
     * @brief Generates a state for the agent based on the current position of the agent.
     * @param agentPosition - The position of the agent.
     * @returns A state that the agent can process.
     */
    State generateState(const glm::ivec2 &agentPosition);
    
    /**
     * @brief Checks to see if the agent is a valid spot in the environment.
     * @param agent - The agent that you want to check.
     * @returns True if the agent is valid, false otherwise.
     */
    bool verifyAgent(Agent &agent) const;
    
    /**
     * @brief Forces and agent to undo it's last move and penalises it.
     * @param agent
     */
    void undoAgent(Agent &agent) const;
    
    /**
     * @brief Rewards the agent based on the state the agent went to.
     * Additionally checks to see if the agent has reached the finish.
     * @param agent - The agent that you want to reward.
     * @param nextState - The next state the agent will be in.
     * @returns True if the agent has reached the finish, false otherwise.
     */
    bool reward(Agent &agent, const State &nextState);
    
    /**
     * @brief Allows ImGui to render and change the variables within the environment.
     */
    void renderOptions();
    
    /**
     * @brief Saves the stats of the environment to the out stream.
     * @param outStream - The stream that you want to save the environment to.
     */
    void serializeRewards(std::ofstream &outStream) const;
    
    /**
     * @brief Loads the data for the environment.
     * @param data - The data that needs to be put into the environment.
     */
    void load(const fileSystem::aiSymbols &data);

protected:
    float   mAgentsLastDistance { 0.f };
    Rewards mRewards;
    
    /**
     * @brief Gets the direction to the goal based on the agent position.
     * @param agentPosition - The agents position you want to compare it to.
     * @returns The direction to the goal "XY". 0 - No Direction, 1 - Up/Left, 2 - Down/Right.
     */
    [[nodiscard]] std::string getGoalDirection(const glm::ivec2 &agentPosition) const;
    
    /**
     * @brief Gets the distance to each of the orthogonal walls.
     * @param agentPosition - The agents position you want to compare it to.
     * @returns The distance to each wall "NESW". 1 - One Away, 2 - Two Away, 3 - Three Away, 4 - Four+ Away.
     */
    [[nodiscard]] std::string getWallDistances(const glm::ivec2 &agentPosition) const;
};


