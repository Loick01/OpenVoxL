#pragma once

#include <variant>

#include "terrain/map_generator.hpp"

enum class ChunkType
{
    Full, Flat, Wave, Editor, Heightmap, Cheese, Cave
};

struct HeightmapData
{
    const unsigned char* values;
    int width;
    int depth;
    int channel;
};

struct SplineData
{
    std::vector<float> plotX;
    std::vector<float> plotY;
    bool use;

    SplineData():
        use(false)
    {
        Reset();
    }

    void Reset() {
        plotX = {0.f, 1.f};
        plotY = {0.f, 1.f};
    }
};

struct DataFullChunk{
    DataFullChunk() {}
};

struct DataFlatChunk{
    DataFlatChunk() {}
};

struct DataWaveChunk
{
    float frequency; // [0.5, 10]
    unsigned int maxBlockHeight;

    DataWaveChunk():
        frequency(1.f), maxBlockHeight(0)
    {}
};

struct DataEditorChunk{
    DataEditorChunk() {}
};

struct DataHeightmapChunk
{
    HeightmapData heightmap;
    NoiseParameters noiseParams;
    SplineData spline;

    DataHeightmapChunk(const NoiseParameters np):
        noiseParams(np)
    {}
};

struct DataCheeseChunk
{
    MapGenerator* generator;
    NoiseParameters noiseParams;
    float threshold; // [-1, 1]

    DataCheeseChunk(MapGenerator* g, const NoiseParameters np):
        generator(g), noiseParams(np), threshold(0.f)
    {}
};

struct DataCaveChunk
{
    HeightmapData heightmap;
    SplineData spline;

    int nrIteration; // [0, 10]
    int threshold; // [0, 8]

    DataCaveChunk():
        nrIteration(3), threshold(4)
    {}
};

using DataChunk = std::variant<
    DataFullChunk,
    DataFlatChunk,
    DataWaveChunk,
    DataEditorChunk,
    DataHeightmapChunk,
    DataCheeseChunk,
    DataCaveChunk
>; 