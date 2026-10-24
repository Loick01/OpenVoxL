#include "core/file/file.hpp"

// template <typename T>
// T FileReader::ReadEnum(std::ifstream& input, const std::unordered_map<std::string,T>& values, const std::string& typeName) const
// {
//     std::string s;
//     input >> s;
//     typename std::unordered_map<std::string, T>::const_iterator it = values.find(s);
//     if (it != values.end())
//         return it->second;

//     Logger::GetInstance().AddLog(LogLevel::Level1, "Unknown value read as " + typeName);
// }

// Axis FileReader::ReadAxis(std::ifstream& input) const
// {
//     static const std::unordered_map<std::string, Axis> axis = {
//         {"width", Axis::Width},
//         {"height", Axis::Height}
//     };

//     return ReadEnum(input, axis, "Axis");
// }

glm::ivec3 FileReader::ReadIVec3(std::ifstream& input) const
{
    glm::ivec3 p;
    input >> p.x >> p.y >> p.z;
    return p;
}