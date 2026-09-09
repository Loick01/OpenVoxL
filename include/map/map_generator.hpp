#pragma once

#include "core/FastNoise.h"

class MapGenerator
{
    private:
        FastNoise m_noise;

        unsigned int m_maxBlockHeight;
        
    public:
        MapGenerator(const int octaves, const int seed, FastNoise::NoiseType type);

        const FastNoise& GetNoise() const;
        int GetOctaves() const;
        int GetSeed() const;
        FastNoise::NoiseType GetNoiseType() const;

        void SetOctaves(const int octaves);
        void SetSeed(const int seed);
        void SetNoiseType(const FastNoise::NoiseType type);

        void GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const;
};