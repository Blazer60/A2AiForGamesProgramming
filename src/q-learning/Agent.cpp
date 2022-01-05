/**
 * @file Agent.cpp
 * @author Ryan Purse
 * @date 31/12/2021
 */


#include "Agent.h"

#include "Common.h"

#include <imgui.h>
#include <fstream>
#include <sstream>


Agent::Agent()
{
    // Creates a Q-Table for every possible state the Agent can be in.
    
    const std::array<std::string, 9> distanceOrdering {
        "00", "01", "02", "10", "11", "12", "20", "21", "22"
    };
    
    const std::array<std::string, 4> dirs { "1", "2", "3", "4" };
    
    // Create every single combination of directions.
    for (const auto &north : dirs)
    {
        for (const auto &east : dirs)
        {
            for (const auto &south : dirs)
            {
                for (const auto &west : dirs)
                {
                    for (const auto &distanceTuple : distanceOrdering)
                    {
                        std::stringstream ss;
                        ss << distanceTuple << north << east << south << west;
                        mQTable.insert( { ss.str(), { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f } } );
                    }
                }
            }
        }
    }
}

glm::ivec2 Agent::performAction(const State &state)
{
    mAction    = exploit(state);
    mPosition += mActionTable.at(mAction);
    
    return mPosition;
}

glm::ivec2 Agent::performTrainingAction(const State &state)
{
    mAction    = chooseAction(state);
    mPosition += mActionTable.at(mAction);
    
    return mPosition;
}

void Agent::undoAction()
{
    mPosition -= mActionTable.at(mAction);
}

void Agent::reward(float points, const State &currState, const State &nextState)
{
    int actionIndex = static_cast<int>(mAction);
    float &bucket   = mQTable.at(currState)[actionIndex];
    float maxOfQ    = getMaxInNextState(nextState);
    
    // Bellman's Equation.
    bucket = (1 - alpha) * bucket + alpha * (points + gamma * maxOfQ);
}

void Agent::renderOptions()
{
    ImGui::Text("Agent Options");
    ImGui::Separator();
    ImGui::DragFloat("Minimum Exploration Rate", &mMinExplorationRate);
    ImGui::DragFloat("Learning Rate", &alpha, 0.01f);
    ImGui::DragFloat("Discount Factor", &gamma, 0.01f);
}

void Agent::serialize(std::ofstream &outStream) const
{
    outStream   << "#xr " << mExplorationRate     << "\n"
                << "#xm " << mMinExplorationRate  << "\n"
                << "#al " << alpha                << "\n"
                << "#ga " << gamma                << "\n";
    
    outStream << "\n";
    for (const auto &[key, value] : mQTable)
    {
        outStream << "#qt " << key << ' ';
        for (const auto &item : value)
        {
            outStream << item << ' ';
        }
        outStream << "\n";
    }
    outStream << "\n";
}

void Agent::load(const fileSystem::aiSymbols &data)
{
    mExplorationRate    = data.explorationRate;
    mMinExplorationRate = data.explorationRateMin;
    alpha               = data.alpha;
    gamma               = data.gamma;
    mQTable             = data.qTable;
}

void Agent::setExplorationRate(float rate)
{
    mExplorationRate = glm::max(mMinExplorationRate, rate);
}

float Agent::getExplorationRate() const
{
    return mExplorationRate;
}

void Agent::setPosition(const glm::ivec2 &position)
{
    mPosition = position;
}

const glm::ivec2 &Agent::getPosition() const
{
    return mPosition;
}

action Agent::chooseAction(const State &state) const
{
    if (randomFloat() < mExplorationRate)
        return explore();
    return exploit(state);
}

action Agent::exploit(const State &state) const
{
    const auto &currentState = mQTable.at(state);
    const uint32_t indexOfLargestValue = std::distance(
            currentState.begin(),
            std::max_element(currentState.begin(), currentState.end())
    );
    return static_cast<action>(indexOfLargestValue);
}

action Agent::explore()
{
    return static_cast<action>(randomInt(0u, static_cast<uint32_t>(action::Count) - 1));
}

float Agent::getMaxInNextState(const State &nextState)
{
    const auto &values = mQTable.at(nextState);
    const auto maxIndex = std::distance(values.begin(), std::max_element(values.begin(), values.end()));
    return values[maxIndex];
}
