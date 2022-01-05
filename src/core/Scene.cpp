/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 01/12/2021
 */


#include "Scene.h"

#include "Pathfinding.h"
#include "MazeLoader.h"
#include "Common.h"
#include "FileIoCommon.h"

#include <imgui_impl_glfw.h>

Scene::Scene(const glm::ivec2 &resolution)
    : mRendererSystem(resolution)
{
    mMazeExplorer.onSelect([this](std::string_view filePath) {
        loadGrid(filePath);
    });
    mAiExplorer.onSelect([this](std::string_view filePath) {
        mPathFinder.loadAi(filePath);
    });
}

void Scene::update()
{
    if (!mIsValidMaze)
        return;
    
    resetGridColour();
    
    switch (mOption)
    {
        case AStar:
            updateAStar();
            break;
        case TrainAi:
            updateTrainAi();
            break;
        case RunAi:
            updateRunAi();
            break;
        case TestAi:
            updateTestAi();
        default:
            break;
    }
    
    colourStartAndFinish();
}

void Scene::render()
{
    RendererSystem::clear();
    if (mIsValidMaze)
        mRendererSystem.draw(mGridMesh->getMesh());
}

void Scene::renderImGui()
{
    showMainMenuBar();
    showSidePanel();
}

void Scene::loadGrid(std::string_view filePath)
{
    mIsValidMaze = false;
    auto data = fileSystem::loadMaze(filePath);
    
    if (data.grid.empty())
    {
        debug::log("The format of the maze does not work. Make sure you are using UTF-8", debug::severity::Minor);
        return;
    }
    
    mGrid     = std::make_shared<Grid>(data.grid, data.gridSize.x);
    mStartPos = mGrid->indexToVector(data.startIndex);
    mEndPos   = mGrid->indexToVector(data.endIndex);
    mGridMesh = std::make_unique<GridMesh>(data.gridSize);
    mMaxPos   = data.gridSize - glm::ivec2(1);
    
    mPathFinder.init(mGrid);
    mPathFinder.resetTraining(mStartPos, mEndPos);
    mIsValidMaze = true;
}

void Scene::updateAStar()
{
    auto [path, gScore] = AStarGridAndInfo<int>(
            mGrid->vectorToIndex(mStartPos), mGrid->vectorToIndex(mEndPos),
            [this](int cell) { return mGrid->getSurrounding(cell); },
            [this](int a, int b) { return mGrid->getDistance(a, b); }
    );
    
    for (const auto &[node, _] : gScore)
        mGridMesh->setCellColour(mGrid->indexToVector(node), glm::vec3(1.f, 0.5f, 0.f));
    
    for (const auto &node : path)
        mGridMesh->setCellColour(mGrid->indexToVector(node), mColours.path);
}

void Scene::updateTrainAi()
{
    if (!mRunTraining)
        return;
    
    const auto path = mPathFinder.trainPath(mStartPos, mEndPos);
    
    for (const auto &node : path)
        mGridMesh->setCellColour(mGrid->indexToVector(node), mColours.path);
    
    moveStartAndFinish();
    
    mStartPos = mGrid->indexToVector(mStartCell);
    mEndPos = mGrid->indexToVector(mEndCell);
    
    if (mPathFinder.isTrainingFinished())
    {
        mRunTraining = false;
        mOption = RunAi;
        const auto saveName = getUniqueString() + ".txt";
        mPathFinder.saveAi("../res/ai/", saveName);
        mAiExplorer.update();
        debug::log("Finished Training AI. Your AI was save to: " + saveName);
    }
}

void Scene::updateRunAi()
{
    const auto path = mPathFinder.calculatePath(mStartPos, mEndPos);
    for (const auto &node : path)
    {
        const auto pos = mGrid->indexToVector(node);
        mGridMesh->setCellColour(pos, mColours.path);
    }
    
}

void Scene::updateTestAi()
{
    if (!mRunTests)
        return;
    
    moveStartAndFinish();
    
    mStartPos = mGrid->indexToVector(mStartCell);
    mEndPos = mGrid->indexToVector(mEndCell);
    
    const auto [aStarTime, aStarPath] = timeIt<std::vector<int>>([this]() {
        return AStarGrid<int>(mStartCell, mEndCell,
                              [this](int cell) { return mGrid->getSurrounding(cell); },
                              [this](int a, int b) { return mGrid->getDistance(a, b); });
    });
    
    const auto [qTime, qPath] = timeIt<std::vector<int>>([this]() {
        return mPathFinder.calculatePath(mStartPos, mEndPos);
    });
    
    mTestLog.aStarTimes.emplace_back(aStarTime);
    mTestLog.aiTimes.emplace_back(qTime);
    mTestLog.aStarPathSize.emplace_back(aStarPath.size());
    mTestLog.aiPathSize.emplace_back(qPath.size());
    
    for (const auto &node : qPath)
        mGridMesh->setCellColour(mGrid->indexToVector(node), mColours.agent);
    for (const auto &node : aStarPath)
        mGridMesh->setCellColour(mGrid->indexToVector(node), mColours.path);
    
    if (++mTestNumber >= mNumberOfTests)
    {
        fileSystem::saveTestData("../res/test-data/results.txt", mTestLog);
        mRunTests = false;
    }
}

void Scene::moveStartAndFinish()
{
    int temp = mGrid->moveToNextValidCell(mStartCell);
    if (temp < mStartCell)
        mEndCell = mGrid->moveToNextValidCell(mEndCell);
    if (temp == mEndCell)
        temp = mGrid->moveToNextValidCell(mStartCell);
    mStartCell = temp;
}

void Scene::resetGridColour()
{
    for (int i = 0; i < mGrid->getCells().size(); ++i)
        mGridMesh->setCellColour(mGrid->indexToVector(i), mGrid->getCells()[i] == 1 ? mColours.wall : mColours.empty);
}

void Scene::colourStartAndFinish()
{
    mGridMesh->setCellColour(mStartPos, mColours.start);
    mGridMesh->setCellColour(mEndPos, mColours.end);
}

void Scene::showAStarSettings()
{
    ImGui::Text("A* Pathfinding Settings");
    ImGui::Separator();
    showStartEndPos();
    
    ImGui::Separator();
}

void Scene::showColourSettings()
{
    if (ImGui::CollapsingHeader("Colour Settings"))
    {
        ImGui::ColorEdit3("Start Colour", &mColours.start.x);
        ImGui::ColorEdit3("End Colour",   &mColours.end.x);
        ImGui::ColorEdit3("Empty Colour", &mColours.empty.x);
        ImGui::ColorEdit3("Wall Colour",  &mColours.wall.x);
        ImGui::ColorEdit3("Path Colour",  &mColours.path.x);
    }
}

void Scene::showLogs()
{
    ImGui::Text("Logs");
    ImGui::Separator();
    ImGui::BeginChild("LogRegion", ImVec2(ImGui::GetWindowContentRegionWidth(), 260), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto &item : debug::getLogs())
        ImGui::Text("%s", item.c_str());
    ImGui::EndChild();
}

void Scene::showMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        std::string text = "Application average %.3f ms/frame (%.1f FPS)";
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
                             - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text(text.c_str(), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }
}

void Scene::showRadioOptions()
{
    ImGui::RadioButton("A* Pathfinding", &mOption, AStar);
    ImGui::SameLine();
    ImGui::RadioButton("Train AI", &mOption, TrainAi);
    ImGui::SameLine();
    ImGui::RadioButton("Run AI", &mOption, RunAi);
    ImGui::SameLine();
    ImGui::RadioButton("Test AI", &mOption, TestAi);
}

void Scene::showRunAiSettings()
{
    ImGui::Text("Run AI Settings");
    ImGui::Separator();
    showStartEndPos();
    mAiExplorer.renderImGui();
}

void Scene::showSidePanel()
{
    const auto* viewPort = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewPort->WorkSize.x - 650, viewPort->WorkPos.y));
    ImGui::SetNextWindowSize(ImVec2(650, viewPort->WorkSize.y));
    
    const ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoCollapse;
    
    
    if (ImGui::Begin("Side Panel", nullptr, windowFlags))
    {
        showRadioOptions();
        switch (mOption)
        {
            case AStar:
                showAStarSettings();
                break;
            case TrainAi:
                showTrainAiSettings();
                break;
            case RunAi:
                showRunAiSettings();
                break;
            case TestAi:
                showTestAiSettings();
            default:
                break;
        }
        
        showLogs();
        mMazeExplorer.renderImGui();
        showColourSettings();
    }
    
    ImGui::End();
}

void Scene::showStartEndPos()
{
    ImGui::SliderInt("Start X", &mStartPos.x, 0, mMaxPos.x);
    ImGui::SliderInt("Start Y", &mStartPos.y, 0, mMaxPos.y);
    
    ImGui::SliderInt("End X", &mEndPos.x, 0, mMaxPos.x);
    ImGui::SliderInt("End Y", &mEndPos.y, 0, mMaxPos.y);
}

void Scene::showTestAiSettings()
{
    ImGui::Text("Test AI Settings");
    ImGui::Separator();
    mAiExplorer.renderImGui();
    ImGui::DragScalar("Number of Tests", ImGuiDataType_U64, &mNumberOfTests, 100);
    if (ImGui::Button("Run Test"))
    {
        mTestLog = fileSystem::TestLog();
        mTestLog.aStarTimes.reserve(mNumberOfTests);
        mTestLog.aiTimes.reserve(mNumberOfTests);
        mTestLog.aiPathSize.reserve(mNumberOfTests);
        mTestLog.aStarPathSize.reserve(mNumberOfTests);
        mTestNumber = 0ull;
        mRunTests = true;
    }
    ImGui::Text("Test Number: %llu", mTestNumber);
}

void Scene::showTrainAiSettings()
{
    ImGui::Text("Train AI Settings");
    ImGui::Separator();
    mPathFinder.renderOptions();
    if (ImGui::Button("Generate New AI"))
    {
        mPathFinder.init(mGrid);
        mRunTraining = true;
    }
    
    mPathFinder.renderTrainingStats();
    mAiExplorer.renderImGui();
}
