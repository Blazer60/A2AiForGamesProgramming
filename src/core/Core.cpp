/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 01/12/2021
 */


#include "Core.h"
#include "Pathfinding.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Core::Core(const glm::ivec2 &resolution)
    : mResolution(resolution)
{
    debug::clearLogs();
    if (!(initGlfw() && initOpenGL() && initImGui()))
    {
        // Failed to load.
        mIsRunning = false;
        debug::log("Failed init. Aborting.", debug::severity::Fatal);
    }
    
    mScene = std::make_unique<Scene>(resolution);
}

Core::~Core()
{
    glfwTerminate();
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
}

void Core::run()
{
    while (mIsRunning)
    {
        update();
        render();
        renderImGui();
    
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        mIsRunning = !glfwWindowShouldClose(mWindow);
    }
}

bool Core::initGlfw()
{
    if (!glfwInit())
    {
        debug::log("Failed to load GLFW.");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // Version of opengl you want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // For debugging
    
    mWindow = glfwCreateWindow(mResolution.x, mResolution.y, "A* ftw", nullptr, nullptr);
    if (!mWindow)
    {
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(mWindow);
    
    return true;
}

bool Core::initOpenGL()
{
    if (glewInit() != GLEW_OK)
    {
        debug::log("Failed to load GLEW");
        return false;
    }
    
    // Debug Messaging.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug::openglCallBack, nullptr);
    
    debug::log(glGetString(GL_VERSION), debug::severity::Notification);
    
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) { return false; }
    
    // Blending texture data / enabling lerping.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    return true;
}

bool Core::initImGui()
{
    ImGui::CreateContext();
    if (ImGui_ImplGlfw_InitForOpenGL(mWindow, true) && ImGui_ImplOpenGL3_Init())
    {
        ImGui::StyleColorsDark();
        return true;
    }
    
    debug::log("Failed to load ImGui");
    return false;
}

void Core::update()
{
    mScene->update();
}

void Core::render()
{
    mScene->render();
}

void Core::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    mScene->renderImGui();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
