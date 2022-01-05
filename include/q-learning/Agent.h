/**
 * @file Agent.h
 * @author Ryan Purse
 * @date 31/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "QlHelpers.h"
#include "AiLoader.h"

#include <map>
#include <random>

/**
 * Performs actions based on the environment it is in. Uses a Q-Table to help decided what is should do.
 * Can be trained to an environment.\n
 * The Q-Table consists of states and actions. An action can either be go in any of the eight directions.
 * A states is defined by the string "XYNESW" where,\n
 * - X is the horizontal direction to the goal (0-No distance, 1-To the left, 2-To the right),\n
 * - Y is the vertical direction to the goal (0-No distance, 1-Above, 2-Below),\n
 * - NESW are the distances to each wall in the corresponding direction (1-One away, 2-Two away, 3-Three away, 4-Four+ away).\n
 * @author Ryan Purse
 * @date 31/12/2021
 */
class Agent
{
public:
    Agent();
    ~Agent() = default;
    
    /**
     * @brief Lets the agent perform an action based on the given state. Will always choose the best value.
     * @param state - The state that the agent is currently in.
     * @returns The new position of the agent.
     */
    glm::ivec2 performAction(const State &state);
    
    /**
     * @brief Let the agent perform an action based on the given state.
     * @param state - The state that the agent is currently in.
     * @returns The new position of the agent.
     */
    glm::ivec2 performTrainingAction(const State &state);
    
    /**
     * @brief Goes back to the previous position.
     */
    void undoAction();
    
    /**
     * @brief Gives a reward to the agent.
     * @param points - How many points you want to give (can be negative).
     * @param currState - Where the agent is.
     * @param nextState - Where the agent went to.
     */
    void reward(float points, const State &currState, const State &nextState);
    
    /**
     * @brief Allows ImGui to show and configure the stats of the Agent.
     */
    void renderOptions();
    
    /**
     * @brief Puts the agent's information into a file stream.
     * @param outStream - The stream where you want the information to go.
     */
    void serialize(std::ofstream &outStream) const;
    
    /**
     * @brief Loads the data for the agent.
     * @param data - all of the data that defines how the agent behaves.
     */
    void load(const fileSystem::aiSymbols &data);
    
    /**
     * @brief Sets the rate at which the agent will choose to explore [0-1].
     */
    void setExplorationRate(float rate);
    
    /**
     * @brief Gets the current exploration rate of the agent.
     */
    [[nodiscard]] float getExplorationRate() const;
    
    /**
     * @brief Sets the position of the agent.
     * @param position - The position (no bounds checking).
     */
    void setPosition(const glm::ivec2 &position);
    
    /**
     * @brief Gets the position of the agent.
     */
    [[nodiscard]] const glm::ivec2 &getPosition() const;

protected:
    QTable     mQTable;
    glm::ivec2 mPosition            { 0 };
    action     mAction              { action::North };
    float      mExplorationRate     { 0.8f };
    float      mMinExplorationRate  { 0.001f };
    float      alpha                { 0.2f };  // The learning rate.
    float      gamma                { 0.9f };  // Discount factor.
    
    // What the agent should do based on the action is chose.
    const std::unordered_map<action, glm::ivec2> mActionTable {
            { action::North,        glm::ivec2( 0, -1) },
            { action::NorthEast,    glm::ivec2( 1, -1) },
            { action::East,         glm::ivec2( 1,  0) },
            { action::SouthEast,    glm::ivec2( 1,  1) },
            { action::South,        glm::ivec2( 0,  1) },
            { action::SouthWest,    glm::ivec2(-1,  1) },
            { action::West,         glm::ivec2(-1,  0) },
            { action::NorthWest,    glm::ivec2(-1, -1) },
    };
    
    /**
     * @brief Chooses an action to perform based on the Q-Table and the exploration rate.
     * @param state - The state that the Agent is currently in.
     * @returns The action that the agent decided to perform.
     */
    [[nodiscard]] action chooseAction(const State &state) const;
    
    /**
     * @brief Looks at the Q-Table and performs the best option based on the state.
     * @param state - The state that the agent in currently in.
     * @returns The action that the agent decided to perform.
     */
    [[nodiscard]] action exploit(const State &state) const;
    
    /**
     * @brief Randomly chooses and action to perform.
     * @returns The action that the agent decided  to perform.
     */
    static action explore();
    
    /**
     * @brief Gets the maximum Q-Value in the next state.
     * @param nextState - the state that you want to look at.
     * @returns maximum value in next state.
     */
    float getMaxInNextState(const State &nextState);
};


