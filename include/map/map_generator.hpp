#pragma once

#include "core/FastNoise.h"

class MapGenerator
{
    private:
        FastNoise m_noise;

        unsigned int m_maxBlockHeight;
        
    public:
        MapGenerator(const int octave, const int seed);

        const FastNoise& GetNoise() const;
        void GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const;
};