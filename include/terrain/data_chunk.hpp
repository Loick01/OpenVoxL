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
    bool use; // TODO : Rename
};

struct DataFullChunk{};
struct DataFlatChunk{};

struct DataWaveChunk
{
    float frequency; // [0.5, 10]
    unsigned int maxBlockHeight;
};

struct DataEditorChunk{};

struct DataHeightmapChunk
{
    HeightmapData heightmap;

    NoiseParameters noiseParams;

    SplineData spline;
};

struct DataCheeseChunk
{
    MapGenerator* generator;
    NoiseParameters noiseParams;
    float threshold; // [-1, 1]
};

struct DataCaveChunk
{
    // HeightmapData heightmap;
    // SplineData spline;
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