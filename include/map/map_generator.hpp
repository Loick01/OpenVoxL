#pragma once

#include <vector>

#include "core/FastNoise.h"

struct NoiseParameters
{
    float frequency;
    int octaves;
    int seed;
    FastNoise::NoiseType noiseType;
};

class MapGenerator
{
    private:
        FastNoise m_noise;
        float m_frequency;
        unsigned int m_maxBlockHeight;
        
        void SetFrequency(const float frequency);
        void SetOctaves(const int octaves);
        void SetSeed(const int seed);
        void SetNoiseType(const FastNoise::NoiseType type);
        
    public:
        MapGenerator(const float frequency, const int octaves, const int seed, FastNoise::NoiseType type);

        NoiseParameters GetNoiseParams() const;
        
        void SetNoiseParams(const NoiseParameters& params);

        float GetNoise3D(const float x, const float y, const float z) const;
        void GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const;
        void GenerateHeightMapWithSpline(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight, 
            const std::vector<float>& plotX, const std::vector<float>& plotY) const; // TODO : Rename plotX and plotY
};