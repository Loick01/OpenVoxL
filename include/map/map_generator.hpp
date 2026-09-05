#pragma once

#include "core/FastNoise.h"

class MapGenerator
{
    private:
        FastNoise m_noise;
        
        unsigned int m_nrBlockWidth;
        unsigned int m_nrBlockDepth;

        unsigned int m_heightBlockTerrainGeneration; // TODO : Rename
        
    public:
        MapGenerator(const unsigned int width, const unsigned int depth, const unsigned int heightBlock, const int octave, const int seed); // TODO : Rename heightBlock

        void GenerateHeightMap() const; // TODO : Rename ?
};