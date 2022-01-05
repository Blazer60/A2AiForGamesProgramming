/**
 * @file FileExplorer.cpp
 * @author Ryan Purse
 * @date 04/01/2022
 */


#include "FileExplorer.h"
#include "FileIoCommon.h"

#include <imgui.h>

FileExplorer::FileExplorer(std::string_view explorerName, std::string_view filePath)
    : mExplorerName(explorerName), mFilePath(filePath)
{
    update();
}

void FileExplorer::update()
{
    mPathItems = fileSystem::ls(mFilePath);
    mIndex = -1;
}

void FileExplorer::renderImGui()
{
    ImGui::Text("%s", mExplorerName.c_str());
    ImGui::Separator();
    if (ImGui::BeginListBox((std::string("##ListBox") + mExplorerName).c_str()))
    {
        for (int n = 0; n < mPathItems.size(); n++)
        {
            const bool is_selected = (mIndex == n);
            if (ImGui::Selectable(mPathItems[n].c_str(), is_selected))
            {
                mIndex = n;
                mOnLoadFunction(mFilePath + "/" + mPathItems[mIndex]);
            }
            
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
}

void FileExplorer::onSelect(const std::function<void(std::string_view)> &onLoad)
{
    mOnLoadFunction = onLoad;
}
