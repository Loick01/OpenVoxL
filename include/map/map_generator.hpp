#pragma once

#include <vector>

#include "core/FastNoise.h"

class MapGenerator
{
    private:
        FastNoise m_noise;
        float m_frequency;
        unsigned int m_maxBlockHeight;
        
    public:
        MapGenerator(const float frequency, const int octaves, const int seed, FastNoise::NoiseType type);

        const FastNoise& GetNoise() const;
        float GetFrequency() const;
        int GetOctaves() const;
        int GetSeed() const;
        FastNoise::NoiseType GetNoiseType() const;

        void SetFrequency(const float frequency);
        void SetOctaves(const int octaves);
        void SetSeed(const int seed);
        void SetNoiseType(const FastNoise::NoiseType type);

        void GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const;
        void GenerateHeightMapWithSpline(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight, 
            const std::vector<float>& plotX, const std::vector<float>& plotY) const; // TODO : Rename plotX and plotY
};