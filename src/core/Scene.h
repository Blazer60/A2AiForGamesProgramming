/**
 * @file Scene.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "Grid.h"
#include "GridMesh.h"
#include "RendererSystem.h"
#include "QlPathFinder.h"
#include "FileExplorer.h"
#include "FileIoCommon.h"

/**
 * @brief Colour definitions that are used to render the grid.
 */
struct Colours
{
    glm::vec3 start { 0.64f, 1.f, 0.64f };
    glm::vec3 end   { 1.f, 0.525f, 0.525f };
    glm::vec3 empty { 0.106f, 0.106f, 0.106f };
    glm::vec3 wall  { 0.f, 0.f, 0.f };
    glm::vec3 path  { 1.f, 0.68f, 0.39f };
    glm::vec3 agent { 0.53f, 0.46f, 0.69f };
};


/**
 * The main 'bulk' of the system. Controls the flow of menus, runs the A* algorithm, trains and runs the AI as well as
 * performing tests to see which is faster.
 * @author Ryan Purse
 * @date 01/12/2021
 */
class Scene
{
    enum option
            : int {
        AStar, TrainAi, RunAi, TestAi
    };
    
public:
    explicit Scene(const glm::ivec2 &resolution);
    ~Scene() = default;
    
    void update();
    void render();
    void renderImGui();
    
protected:
    /** Enum option for what part of the code should be run. */
    int mOption { 0 };
    
    /** The Q-Learning Pathfinder. A* is a single function */
    QlPathFinder mPathFinder;
    
    /** All of the colours that can be renderer to the grid. */
    Colours mColours;
    
    /** All of the test data that is logged to a file. Q-learning Vs. A* */
    fileSystem::TestLog mTestLog;
    
    /** Handles rendering of meshes. */
    RendererSystem mRendererSystem;
    
    /** The actual grid that is stored. */
    std::shared_ptr<Grid> mGrid;
    
    /** The mesh representation of the grid. */
    std::unique_ptr<GridMesh> mGridMesh;
    
    /** Determines whether the maze if valid or not. Nothing will happen while this is set to false. */
    bool mIsValidMaze { false };
    
    /** The start position for both A* and Q-learning. */
    glm::ivec2 mStartPos { 0, 0 };
    
    /** The finish position for both A* and Q-learning. */
    glm::ivec2 mEndPos { 0, 0 };
    
    /** The maximum position that both start and finish position can be. */
    glm::ivec2 mMaxPos { 0 };
    
    /** Index form of the start position. Used for iterating */
    int mStartCell { 0 };
    
    /** Index form of the finish position. Used for iterating. */
    int mEndCell { 0 };
    
    /** An explorer for ImGui to show all of the mazes that can be loaded. */
    FileExplorer mMazeExplorer { "Mazes", "../res/mazes" };
    
    /** An explorer for ImGui to show all of the Ai 'brains' that can be loaded. */
    FileExplorer mAiExplorer { "AI", "../res/ai" };
    
    /** Determines whether the training cycle should continue or should be aborted. */
    bool mRunTraining { false };
    
    /** Determines whether the testing cycle should continue or should be aborted. */
    bool mRunTests { false };
    
    /** The current test number that it's on. */
    uint64_t mTestNumber { 0ull };
    
    /** The maximum number of tests it will do. */
    uint64_t mNumberOfTests { 10'000ull };
    
    /**
     * @brief Attempts to load a maze from the given file path. Sets mIsValidMaze to false it fails.
     * @param filePath - A path to where the maze is stored.
     */
    void loadGrid(std::string_view filePath);
    
    /**
     * @brief Performs the A* Algorithm on the loaded grid.
     */
    void updateAStar();
    
    /**
     * @brief Trains the Ai. Switches to run Ai when it's finished.
     */
    void updateTrainAi();
    
    /**
     * @brief Runs the Ai.
     */
    void updateRunAi();
    
    /**
     * @brief Tests the Ai.
     */
    void updateTestAi();
    
    /**
     * @brief Moves the start and finish around iteratively. For Training the Ai and testing the Ai.
     */
    void moveStartAndFinish();
    
    /**
     * @brief Resets the grid colours to their original colours.
     */
    void resetGridColour();
    
    /**
     * @brief Colours the start and finished based on the colours set in mColours.
     */
    void colourStartAndFinish();
    
    // The following render said item to ImGui so that the user can interact with the program.
    // Calling them in a different order can yield in different results.
    
    void showAStarSettings();
    void showColourSettings();
    void showLogs();
    void showMainMenuBar();
    void showRadioOptions();
    void showRunAiSettings();
    void showSidePanel();
    void showStartEndPos();
    void showTestAiSettings();
    void showTrainAiSettings();
};


