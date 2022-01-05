/**
 * @file Environment.cpp
 * @author Ryan Purse
 * @date 31/12/2021
 */


#include "Environment.h"

#include "Grid.h"
#include "Agent.h"

#include <fstream>
#include <imgui.h>

State Environment::generateState(const glm::ivec2 &agentPosition)
{
    State newState  = getGoalDirection(agentPosition);
    newState       += getWallDistances(agentPosition);
    return newState;
}

bool Environment::verifyAgent(Agent &agent) const
{
    return grid->verifyCell(grid->vectorToIndex(agent.getPosition()));
}

void Environment::undoAgent(Agent &agent) const
{
    agent.undoAction();
    agent.reward(mRewards.intoWall, state, state);
}

bool Environment::reward(Agent &agent, const State &nextState)
{
    const glm::ivec2 &agentPosition = agent.getPosition();
    if (agentPosition == goal)
    {
        agent.reward(mRewards.goal, state, nextState);
        return true;
    }
    
    float currentDistanceToFinish = glm::distance(static_cast<glm::vec2>(agentPosition), static_cast<glm::vec2>(goal));
    bool isCloser = currentDistanceToFinish < mAgentsLastDistance;
    mAgentsLastDistance = currentDistanceToFinish;
    agent.reward(isCloser ? mRewards.towardsGoal : mRewards.awayFromGoal, state, nextState);
    return false;
}

void Environment::renderOptions()
{
    ImGui::Text("Reward Options");
    ImGui::Separator();
    ImGui::DragFloat("Goal Reward",      &mRewards.goal);
    ImGui::DragFloat("Towards the goal", &mRewards.towardsGoal);
    ImGui::DragFloat("Away from goal",   &mRewards.awayFromGoal);
    ImGui::DragFloat("Into Wall",        &mRewards.intoWall);
}

void Environment::serializeRewards(std::ofstream &outStream) const
{
    outStream   << "#rg " << mRewards.goal         << "\n"
                << "#rf " << mRewards.towardsGoal  << "\n"
                << "#rb " << mRewards.awayFromGoal << "\n"
                << "#rw " << mRewards.intoWall     << "\n";
}

void Environment::load(const fileSystem::aiSymbols &data)
{
    mRewards.goal         = data.rewardGoal;
    mRewards.towardsGoal  = data.rewardTowardGoal;
    mRewards.awayFromGoal = data.rewardAwayFromGoal;
    mRewards.intoWall     = data.rewardIntoWall;
}

std::string Environment::getGoalDirection(const glm::ivec2 &agentPosition) const
{
    // Check which direction the goal is on the x-axis.
    std::string xDir(1, static_cast<char>(direction::NoDirection));
    if (agentPosition.x < goal.x)
        xDir = static_cast<char>(direction::Right);
    else if (agentPosition.x > goal.x)
        xDir = static_cast<char>(direction::Left);
    
    // Check which direction the goal is on the y-axis.
    std::string yDir(1, static_cast<char>(direction::NoDirection));
    if (agentPosition.y < goal.y)
        yDir = static_cast<char>(direction::Down);
    else if (agentPosition.y > goal.y)
        yDir = static_cast<char>(direction::Up);
    
    return xDir + yDir;  // Concatenate the two to give us the first part of our state.
}

std::string Environment::getWallDistances(const glm::ivec2 &agentPosition) const
{
    const std::vector<int> wallDistances = grid->getDistanceToOrthogonalWalls(grid->vectorToIndex(agentPosition));
    
    std::string nesw = "xxxx";  // North, East, South, West
    const std::string_view ranks = "1234";
    
    for (int i = 0; i < 4; ++i)
    {
        const int clampedDistance = glm::clamp(wallDistances[i], 0, 4);
        nesw[i] = ranks[clampedDistance - 1];
    }
    
    return nesw;
}

