#include "core/file/file.hpp"

#include "core/file/path.hpp"
#include "debug/logger.hpp"

StructureData FileReader::ReadStructureFile(const std::string& structFilepath) const
{
    std::ifstream input = OpenFile(std::string(DataDirectory::Structure) + structFilepath + std::string(FileExtension::Structure));
    StructureData data;

    int id;
    while (input >> id)
        data.blocks.emplace_back(id, ReadIVec3(input));

    input.close();
    return data;
}

void FileReader::SaveStructureFile(const std::string& structFilepath, const StructureData& structData) const
{
    if (structFilepath.empty()) {
        Logger::GetInstance().AddLog(LogLevel::Level1, "Can't save the structure --> File name is empty");
        return;
    }

    const std::string filepath = std::string(DataDirectory::Structure) + structFilepath + std::string(FileExtension::Structure);
    std::ofstream output(filepath);

    for (const BlockData& block : structData.blocks)
        output << block.id << " " << block.position.x << " " << block.position.y << " " << block.position.z << "\n"; 

    output.close();
    Logger::GetInstance().AddLog(LogLevel::Level1, "The structure has been saved as " + filepath);
}