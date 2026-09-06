#pragma once

#include "core/FastNoise.h"

class MapGenerator
{
    private:
        FastNoise m_noise;
        
        unsigned int m_nrBlockWidth;
        unsigned int m_nrBlockDepth;

        unsigned int m_maxBlockHeight;
        
    public:
        MapGenerator(const unsigned int width, const unsigned int depth, const unsigned int maxBlockHeight, const int octave, const int seed);

        const FastNoise& GetNoise() const;
        void GenerateHeightMap() const;
};