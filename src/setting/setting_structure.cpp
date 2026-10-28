#include "setting/setting.hpp"

#include <dirent.h> // opendir

#include "core/file/file.hpp"
#include "core/file/path.hpp"
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

            Logger::GetInstance().AddLog(LogLevel::Level0, "This structure file has been found : " + name); // TODO : Remove

            if (std::find(m_loadedStructures.begin(), m_loadedStructures.end(), structureInfo) == m_loadedStructures.end()) {
                structureInfo.iconTextureId = LoadTexture2D(directoryPath + name + std::string(FileExtension::Image));
                m_loadedStructures.push_back(structureInfo);
            }
        }
    }
    
    closedir(structureDirectory);
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