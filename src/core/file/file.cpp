#include "core/file/file.hpp"

#include "core/file/path.hpp"

StructureData FileReader::ReadStructureFile(const std::string& structFilepath) const
{
    std::ifstream input = OpenFile(std::string(DataDirectory::Structure) + structFilepath);
    StructureData data;

    int id;
    while (input >> id)
        data.blocks.emplace_back(id, ReadIVec3(input));

    input.close();
    return data;
}

void FileReader::SaveStructureFile(const std::string& structFilepath, const StructureData& structData) const
{
    std::ofstream output(std::string(DataDirectory::Structure) + structFilepath);

    for (const BlockData& block : structData.blocks)
        output << block.id << " " << block.position.x << " " << block.position.y << " " << block.position.z << "\n"; 

    output.close();
}