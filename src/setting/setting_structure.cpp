#include "setting/setting.hpp"

#include <dirent.h> // opendir

#include "core/file/file.hpp"
#include "core/file/path.hpp"
#include "graphic/stb_image_write.h"
#include "graphic/texture.hpp"

StructureData SettingWindow::GetSelectedStructure() const
{
    if (m_loadedStructures.empty()) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "No structures loaded");
        return StructureData{};
    }

    const std::string structName = m_loadedStructures[m_selectedStructure].name;
    return m_fileReader.ReadStructureFile(structName);
}

void SettingWindow::OpenStructureDirectory()
{   
    m_loadedStructures.clear();
    
    const std::string directoryPath = std::string(DataDirectory::Structure);
    
    DIR* structureDirectory = opendir(directoryPath.c_str());
    if (structureDirectory == nullptr) {
        Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to open the directory : " + directoryPath);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(structureDirectory)) != nullptr) {
        if (entry->d_type != DT_REG)
            continue;

        const std::string structureFilename = entry->d_name;
        if (m_fileReader.IsStructFile(structureFilename)) {
            const std::string name = m_fileReader.GetFileNameWithoutExtension(structureFilename);
            StructureInfo structureInfo{name, 0};

            if (std::find(m_loadedStructures.begin(), m_loadedStructures.end(), structureInfo) == m_loadedStructures.end()) {
                structureInfo.iconTextureId = LoadTexture2D(directoryPath + name + std::string(FileExtension::Image));
                m_loadedStructures.push_back(structureInfo);
            }
        }
    }
    
    closedir(structureDirectory);
}

void SettingWindow::WriteStructureIcon(const std::string& name, const unsigned int iconSize)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_glfwWindow, &windowWidth, &windowHeight);
    const ImVec2 position = ImVec2((windowWidth-iconSize)/2, (windowHeight-iconSize)/2); 
    unsigned char* pixels = new unsigned char[3*iconSize*iconSize];
    const std::string fullPath = std::string(DataDirectory::Structure) + name + std::string(FileExtension::Image);

    glReadPixels(position.x, position.y, iconSize, iconSize, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    stbi_flip_vertically_on_write(true);
    stbi_write_png(fullPath.c_str(), iconSize, iconSize, 3, pixels, iconSize*3); // TODO : Use stbi_write_png_compression_level ?
    stbi_flip_vertically_on_write(false);

    delete[] pixels;
}

void SettingWindow::ShowStructureIcons()
{
    if (m_loadedStructures.empty()) {
        ImGui::Text("No structures in %s", DataDirectory::Structure.data());
        return;
    } else {
        ImGui::Text("Current structure : %s", m_loadedStructures[m_selectedStructure].name.c_str());
    }
    
    ImGui::Columns(3, "structuresIcon", false);
    for (unsigned int i = 0 ; i < 3 ; i++)
        ImGui::SetColumnWidth(i, 144); 

    for (unsigned int i = 0 ; i < m_loadedStructures.size() ; i++) {
        const StructureInfo& structure = m_loadedStructures[i];
        ImGui::Image((ImTextureID)(intptr_t)structure.iconTextureId, ImVec2(128, 128));
        
        if (ImGui::Button(structure.name.c_str(), ImVec2(128, 24))) {
            m_selectedStructure = i;
            Notify(Setting::UpdateValues);
        }

        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}