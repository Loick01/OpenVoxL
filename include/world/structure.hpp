#pragma once

#include <vector>

#include <glm/glm.hpp>

struct BlockData
{
    glm::ivec3 position;
    unsigned int id;

    BlockData(const unsigned int i, const glm::ivec3 p):
        id(i), position(p)
    {}
};

struct StructureData 
{
    std::vector<BlockData> blocks;
    // TODO : const std::string name ?

    void RotateToLeft()
    {
        for (BlockData& block : blocks) {
            const unsigned int t = block.position.z;
            block.position.z = block.position.x;
            block.position.x = -t;
        }
    }
    
    void RotateToRight()
    {
        for (BlockData& block : blocks) {
            const unsigned int t = block.position.x;
            block.position.x = block.position.z;
            block.position.z = -t;
        }
    } 
};