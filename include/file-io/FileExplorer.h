/**
 * @file FileExplorer.h
 * @author Ryan Purse
 * @date 04/01/2022
 */


#pragma once

#ifdef NO_PCH
#include "Pch.h"
#endif  // NO_PCH

#include <functional>

/**
 * Allows a file explorer to be rendered to ImGui based on the path provided.
 * @author Ryan Purse
 * @date 04/01/2022
 */
class FileExplorer
{
public:
    /**
     * @brief Creates a file explorer. Call onSelect to make the file explorer interactive.
     * @param explorerName - A unique Id for ImGui to render it correctly.
     * @param filePath - A path to a folder to all of the items that you want to show.
     */
    FileExplorer(std::string_view explorerName, std::string_view filePath);
    
    /**
     * @brief Updates the list of items that are shown.
     */
    void update();
    
    /**
     * @brief Renders the file explorer to the current ImGui Window.
     */
    void renderImGui();
    
    /**
     * @brief A callback function to make the file explorer interactive.
     * @param onLoad - void(std::string_view path) where path is the path to a selected path.
     */
    void onSelect(const std::function<void(std::string_view)> &onLoad);
    
protected:
    std::function<void(std::string_view)> mOnLoadFunction { [](std::string_view){} };
    
    std::string mExplorerName;
    std::string mFilePath;
    std::vector<std::string> mPathItems;
    int mIndex { -1 };
};


