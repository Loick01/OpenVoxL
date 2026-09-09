#include "map/map_generator.hpp"

#include <cstdlib>

#include "graphic/stb_image_write.h"

MapGenerator::MapGenerator(const int octaves, const int seed, FastNoise::NoiseType type)
{
    SetOctaves(octaves);
    SetSeed(seed);
    SetNoiseType(type);
}

const FastNoise& MapGenerator::GetNoise() const
{
    return m_noise;
}

int MapGenerator::GetOctaves() const
{
    return m_noise.GetFractalOctaves();
}

int MapGenerator::GetSeed() const
{
    return m_noise.GetSeed();
}

FastNoise::NoiseType MapGenerator::GetNoiseType() const
{
    return m_noise.GetNoiseType();
}

void MapGenerator::SetOctaves(const int octaves)
{
    m_noise.SetFractalOctaves(octaves);
}

void MapGenerator::SetSeed(const int seed)
{
    m_noise.SetSeed(seed);
}

void MapGenerator::SetNoiseType(const FastNoise::NoiseType type)
{
    m_noise.SetNoiseType(type);
}

void MapGenerator::GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const
{
    const unsigned int dataSize = nrBlockWidth*nrBlockDepth;
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);

    for(unsigned int j = 0 ; j < nrBlockDepth ; j++) { // Z
        for(unsigned int i = 0 ; i < nrBlockWidth ; i++) { // X
            const unsigned int height = ((m_noise.GetNoise(i,j)+1)/2)*(maxBlockHeight-1);
            data[j*nrBlockWidth+i] = height;
        }
    }

    stbi_write_png("../data/heightmap/terrain.png", nrBlockWidth, nrBlockDepth, 1, data, nrBlockWidth);
    free(data);
}