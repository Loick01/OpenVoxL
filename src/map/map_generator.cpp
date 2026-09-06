#include "map/map_generator.hpp"

#include <cstdlib>

#include "graphic/stb_image_write.h"

MapGenerator::MapGenerator(const unsigned int width, const unsigned int depth, const unsigned int maxBlockHeight, const int octave, const int seed):
    m_nrBlockWidth(width), m_nrBlockDepth(depth), m_maxBlockHeight(maxBlockHeight)
{
    // enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };
    m_noise.SetNoiseType(FastNoise::SimplexFractal);
    m_noise.SetFractalOctaves(octave);
    m_noise.SetSeed(seed);
}

const FastNoise& MapGenerator::GetNoise() const
{
    return m_noise;
}

void MapGenerator::GenerateHeightMap() const
{
    const unsigned int dataSize = m_nrBlockWidth*m_nrBlockDepth;
    unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);

    for(unsigned int j = 0 ; j < m_nrBlockDepth ; j++) { // Z
        for(unsigned int i = 0 ; i < m_nrBlockWidth ; i++) { // X
            const unsigned int height = ((m_noise.GetNoise(i,j)+1)/2)*(m_maxBlockHeight-1);
            data[j*m_nrBlockWidth+i] = height;
        }
    }

    stbi_write_png("../data/heightmap/terrain.png", m_nrBlockWidth, m_nrBlockDepth, 1, data, m_nrBlockWidth);
    free(data);
}