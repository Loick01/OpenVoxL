#include "map/map_generator.hpp"

#include <cstdlib>

#include "graphic/stb_image_write.h"

MapGenerator::MapGenerator(const unsigned int width, const unsigned int depth, const unsigned int heightBlock, const int octave, const int seed):
    m_nrBlockWidth(width), m_nrBlockDepth(depth), m_heightBlockTerrainGeneration(heightBlock)
{
    // enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };
    m_noise.SetNoiseType(FastNoise::SimplexFractal);
    m_noise.SetFractalOctaves(octave);
    m_noise.SetSeed(seed);

    GenerateHeightMap();
}

void MapGenerator::GenerateHeightMap() const
{
    const unsigned int dataSize = m_nrBlockWidth*m_nrBlockDepth;
    unsigned char* dataPixels = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);

    for(unsigned int j = 0 ; j < m_nrBlockDepth ; j++) { // Z
        for(unsigned int i = 0 ; i < m_nrBlockWidth ; i++) { // X
            float value = ((m_noise.GetNoise(i,j)+1)/2)*(m_heightBlockTerrainGeneration-1);
            dataPixels[j*m_nrBlockWidth+i] = value;
        }
    }

    stbi_write_png("../data/terrain.png", m_nrBlockWidth, m_nrBlockDepth, 1, dataPixels, m_nrBlockWidth);
    free(dataPixels);
}