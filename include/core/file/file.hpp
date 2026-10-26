#pragma once

#include <fstream>
#include <string>

#include "debug/logger.hpp"
#include "terrain/structure.hpp"

#define SECTION_DELIMITER "###" // define ? Rename
#define TEXT_DELIMITER ';' // define ? Rename

class FileReader // TODO : Should be Singleton ?
{
    private:
        // FileIO
        static std::ifstream OpenFile(const std::string& filepath);
        std::string ReadString(std::ifstream& input) const;

        // FileUtils
        // template <typename T>
        //     T ReadEnum(std::ifstream& input, const std::unordered_map<std::string, T>& values, const std::string& typeName) const;
        // Axis ReadAxis(std::ifstream& input) const;
        glm::ivec3 ReadIVec3(std::ifstream& input) const;
        
        // File

    public:
        FileReader() = default;

        // File
        StructureData ReadStructureFile(const std::string& structFilepath) const;
        void SaveStructureFile(const std::string& structFilepath, const StructureData& structData) const;
        
        // FileIO
        std::string GetFileExtension(const std::string& filepath) const;
        std::string GetFileNameWithoutExtension(const std::string& filepath) const;
        bool IsStructFile(const std::string& filepath) const;
};