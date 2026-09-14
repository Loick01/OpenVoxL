#pragma once

#include <vector>

#include "core/FastNoise.h"

struct NoiseParameters
{
    float frequency;
    int octaves;
    int seed;
    FastNoise::NoiseType noiseType;
    FastNoise::Interp interp; // Used in Value and Perlin
    FastNoise::FractalType fractalType; // Used in ValueFractal, PerlinFractal, SimplexFractal and CubicFractal
    FastNoise::CellularDistanceFunction cellularDistanceFunction; // Used in Cellular
    FastNoise::CellularReturnType cellularReturnType; // Used in Cellular

    // enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal }; Default = Simplex
	// enum Interp { Linear, Hermite, Quintic };                                                                                          Default = Quintic
	// enum FractalType { FBM, Billow, RigidMulti };                                                                                      Default = FBM
	// enum CellularDistanceFunction { Euclidean, Manhattan, Natural };                                                                   Default = Euclidean
	// enum CellularReturnType { CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };   Default = CellValue
};

class MapGenerator
{
    private:
        FastNoise m_noise;
        float m_frequency;
        unsigned int m_maxBlockHeight;
        
    public:
        MapGenerator(const float frequency, const int octaves, const int seed, FastNoise::NoiseType type);

        NoiseParameters GetNoiseParams() const;
        
        void SetNoiseParams(const NoiseParameters& params);

        float GetNoise3D(const float x, const float y, const float z) const;
        void GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const;
        void GenerateHeightMapWithSpline(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight, 
            const std::vector<float>& plotX, const std::vector<float>& plotY) const;
};