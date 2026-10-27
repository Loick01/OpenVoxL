#include "setting/setting.hpp"

#include <dirent.h> // opendir

#include "core/file/file.hpp"
#include "core/file/path.hpp"
#include "graphic/texture.hpp"

void SettingWindow::OpenStructureDirectory()
{   
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
            const std::string structureName = m_fileReader.GetFileNameWithoutExtension(structureFilename);
            StructureInfo structureInfo{structureName, 0};

            Logger::GetInstance().AddLog(LogLevel::Level0, "This structure file has been found : " + structureName); // TODO : Remove

            if (std::find(m_loadedStructures.begin(), m_loadedStructures.end(), structureInfo) == m_loadedStructures.end()) {
                structureInfo.iconTextureId = LoadTexture2D(directoryPath + structureName + std::string(FileExtension::Image));
                m_loadedStructures.push_back(structureInfo);
            }
        }
    }
    
    closedir(structureDirectory);
}