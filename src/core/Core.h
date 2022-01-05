/**
 * @file Core.h
 * @author Ryan Purse
 * @date 01/12/2021
 */


#pragma once

#include <glew.h>  // This needs to be the first include before using any opengl libs
#include <glfw3.h>

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include "Scene.h"

/**
 * Runs and holds the main loop of the program.
 * @author Ryan Purse
 * @date 01/12/2021
 */
class Core
{
public:
    explicit Core(const glm::ivec2 &resolution={ 1280, 720 });
    ~Core();
    
    /** Runs the core loop of the program. */
    void run();
    
protected:
    std::unique_ptr<Scene>  mScene;
    glm::ivec2              mResolution;
    GLFWwindow              *mWindow    { nullptr };
    bool                    mIsRunning  { true };
    
    /**
     * @brief Initialises GLFW.
     * @returns True if success, false otherwise.
     */
    bool initGlfw();
    
    /**
     * @brief Initialises OpenGl
     * @returns True if success, false otherwise.
     */
    bool initOpenGL();
    
    /**
     * @brief Initialises ImGui.
     * @returns True if success, false otherwise.
     */
    bool initImGui();
    
    void update();
    void render();
    void renderImGui();
};


