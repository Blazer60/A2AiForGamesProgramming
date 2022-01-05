/**
 * @file QlPathFinder.cpp
 * @author Ryan Purse
 * @date 02/01/2022
 */


#include "QlPathFinder.h"

#include "Grid.h"
#include "FileIoCommon.h"
#include "AiLoader.h"

#include <imgui.h>
#include <fstream>

void QlPathFinder::init(const std::shared_ptr<Grid>& grid)
{
    mEnvironment.grid = grid;
    mEpisode = 0ull;
}

void QlPathFinder::update()
{
    mAgent.performAction(mEnvironment.state);
    if (!mEnvironment.verifyAgent(mAgent))
        mAgent.undoAction();
    
    mEnvironment.state = mEnvironment.generateState(mAgent.getPosition());
    mIsEpisodeFinished = mEnvironment.goal == mAgent.getPosition() || mIteration++ >= mIterationMax;
}

void QlPathFinder::updateTrain()
{
    mAgent.performTrainingAction(mEnvironment.state);
    if (!mEnvironment.verifyAgent(mAgent))
        mEnvironment.undoAgent(mAgent);
    
    State nextState    = mEnvironment.generateState(mAgent.getPosition());
    mIsEpisodeFinished = mEnvironment.reward(mAgent, nextState) || mIteration++ >= mIterationMax;
    mEnvironment.state = nextState;
}

std::vector<int> QlPathFinder::calculatePath(const glm::ivec2 &start, const glm::ivec2 &end)
{
    reset(start, end);
    
    std::vector<int> path { mEnvironment.grid->vectorToIndex(start) };
    
    while (!mIsEpisodeFinished)
    {
        update();
        const auto pos = mAgent.getPosition();
        path.push_back(mEnvironment.grid->vectorToIndex(pos));
    }
    
    return path;
}

std::vector<int> QlPathFinder::trainPath(const glm::ivec2 &start, const glm::ivec2 &end)
{
    resetTraining(start, end);
    
    std::vector<int> path { mEnvironment.grid->vectorToIndex(start) };
    
    while(!mIsEpisodeFinished)
    {
        updateTrain();
        path.push_back(mEnvironment.grid->vectorToIndex(mAgent.getPosition()));
    }
    
    return path;
}

void QlPathFinder::reset(const glm::ivec2 &start, const glm::ivec2 &end)
{
    mAgent.setPosition(start);
    mEnvironment.goal = end;
    mEnvironment.state = mEnvironment.generateState(mAgent.getPosition());
    mIteration = 0;
    mIsEpisodeFinished = false;
}

void QlPathFinder::resetTraining(const glm::ivec2 &start, const glm::ivec2 &end)
{
    mAgent.setPosition(start);
    mEnvironment.goal = end;
    mEnvironment.state = mEnvironment.generateState(mAgent.getPosition());
    mIsEpisodeFinished = false;
    mEpisode++;
    const float percentage = static_cast<float>(mEpisode) / static_cast<float>(mEpisodeMax);
    mAgent.setExplorationRate(glm::smoothstep(1.f, 0.f, percentage));
    mIteration = 0;
}

void QlPathFinder::renderOptions()
{
    ImGui::Text("AI Options");
    ImGui::Separator();
    ImGui::DragScalar("Maximum Episodes", ImGuiDataType_U64, &mEpisodeMax, 100.f);
    ImGui::DragScalar("Maximum Iterations", ImGuiDataType_U64, &mIterationMax, 100.f);
    mAgent.renderOptions();
    mEnvironment.renderOptions();
}

void QlPathFinder::renderTrainingStats()
{
    ImGui::Text("Episode: %llu", mEpisode);
    ImGui::Text("Iteration: %llu", mIteration);
    ImGui::Text("Exploration Rate: %f", mAgent.getExplorationRate());
}

bool QlPathFinder::isTrainingFinished() const
{
    return mEpisode >= mEpisodeMax;
}

void QlPathFinder::saveAi(std::string_view path, std::string_view name) const
{
    std::ofstream outStream;
    outStream.open(std::string(path) + std::string(name));
    if (outStream.fail() || outStream.bad())
    {
        debug::log("The file could not be opened", debug::severity::Minor);
        return;
    }
    
    outStream.clear();
    
    outStream   << "#ve " << mVersion       << "\n"
                << "#ep " << mEpisode       << "\n"
                << "#em " << mEpisodeMax    << "\n"
                << "#im " << mIterationMax  << "\n";
    mEnvironment.serializeRewards(outStream);
    mAgent.serialize(outStream);
    
    outStream.close();
}

void QlPathFinder::loadAi(std::string_view path)
{
    const fileSystem::aiSymbols data = fileSystem::loadAi(path);
    mIteration = 0;
    
    mEpisode                = data.episode;
    mEpisodeMax             = data.episodeMax;
    mIterationMax           = data.iterationMax;
    
    mEnvironment.load(data);
    mAgent.load(data);
}
