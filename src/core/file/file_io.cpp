#include "core/file/file.hpp"

#include <algorithm>

#include "core/file/path.hpp"

std::ifstream FileReader::OpenFile(const std::string& filepath)
{
    std::ifstream input;
    input.open(filepath);
    if (!input)
        Logger::GetInstance().AddLog(LogLevel::Level2, "Failed to open the file : " + filepath);
    
    return input;
}

std::string FileReader::ReadString(std::ifstream& input) const 
{
    std::string s;
    input >> s;
    std::replace(s.begin(), s.end(), '_', ' ');
    return s;
}

std::string FileReader::GetFileExtension(const std::string& filepath) const 
{
    size_t pos = filepath.rfind('.');
    if (pos == std::string::npos) {
        Logger::GetInstance().AddLog(LogLevel::Level1, "This file has no extension : " + filepath);
        return "";
    }

    return filepath.substr(pos);
}

std::string FileReader::GetFileNameWithoutExtension(const std::string& filepath) const
{
    size_t pos = filepath.rfind('.');
    if (pos == std::string::npos)
        Logger::GetInstance().AddLog(LogLevel::Level1, "This file has no extension : " + filepath);

    return filepath.substr(0, pos);
}

bool FileReader::IsStructFile(const std::string& filepath) const
{
    return GetFileExtension(filepath) == FileExtension::Structure;
}