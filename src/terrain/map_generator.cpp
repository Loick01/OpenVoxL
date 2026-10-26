#include "terrain/map_generator.hpp"

#include <cstdlib>

#include "graphic/stb_image_write.h"

MapGenerator::MapGenerator(const float frequency, const int octaves, const int seed, FastNoise::NoiseType type):
    m_frequency(frequency)
{
    m_noise.SetFractalOctaves(octaves);
    m_noise.SetSeed(seed);
    m_noise.SetNoiseType(type);
}

NoiseParameters MapGenerator::GetNoiseParams() const
{
    return {m_frequency, m_noise.GetFractalOctaves(), m_noise.GetSeed(), m_noise.GetNoiseType()};
}

void MapGenerator::SetNoiseParams(const NoiseParameters& params)
{
    m_frequency = params.frequency;
    m_noise.SetFractalOctaves(params.octaves);
    m_noise.SetSeed(params.seed);
    m_noise.SetNoiseType(params.noiseType);
    m_noise.SetInterp(params.interp);
    m_noise.SetFractalType(params.fractalType);
    m_noise.SetCellularDistanceFunction(params.cellularDistanceFunction);
    m_noise.SetCellularReturnType(params.cellularReturnType);
}

float MapGenerator::GetNoise3D(const float x, const float y, const float z) const
{
    return m_noise.GetNoise(x, y, z);
}

void MapGenerator::GenerateHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight) const
{
    std::vector<unsigned char> grid(nrBlockWidth*nrBlockDepth);

    for(unsigned int j = 0 ; j < nrBlockDepth ; j++) { // Z
        for(unsigned int i = 0 ; i < nrBlockWidth ; i++) { // X
            const unsigned int height = ((m_noise.GetNoise(i*m_frequency,j*m_frequency)+1)/2)*(maxBlockHeight-1);
            grid[j*nrBlockWidth+i] = height;
        }
    }

    stbi_write_png("../data/heightmap/terrain.png", nrBlockWidth, nrBlockDepth, 1, grid.data(), nrBlockWidth);
}

void MapGenerator::UseSplineForHeight(std::vector<unsigned char>& grid, const unsigned int width, const unsigned int depth, const unsigned int maxHeight,
    const std::vector<float>& plotX, const std::vector<float>& plotY) const
{
    for(unsigned int j = 0 ; j < depth ; j++) { // Z
        for(unsigned int i = 0 ; i < width ; i++) { // X
            const float v = ((m_noise.GetNoise(i*m_frequency,j*m_frequency)+1)/2); // TODO : Add a frequency parameter

            float i1, i2;
            for (unsigned int i = 0 ; i < plotX.size()-1 ; i++){
                if (v >= plotX[i] && v <= plotX[i+1]) {
                    i1 = i;
                    i2 = i+1;
                    break;
                }
            }

            const float f = plotY[i1] + (v - plotX[i1])*((plotY[i2]-plotY[i1])/(plotX[i2]-plotX[i1]));

            grid[j*width+i] *= f * (maxHeight-1);
        }
    }
}

void MapGenerator::GenerateHeightMapWithSpline(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight, 
    const std::vector<float>& plotX, const std::vector<float>& plotY) const
{
    std::vector<unsigned char> grid(nrBlockWidth*nrBlockDepth, 1);
    UseSplineForHeight(grid, nrBlockWidth, nrBlockDepth, maxBlockHeight, plotX, plotY);
    stbi_write_png("../data/heightmap/terrain.png", nrBlockWidth, nrBlockDepth, 1, grid.data(), nrBlockWidth);
}

void MapGenerator::GenerateCaveHeightMap(const unsigned int nrBlockWidth, const unsigned int nrBlockDepth, const unsigned int maxBlockHeight,
    const unsigned int nrIteration, const unsigned int threshold, const std::vector<float>& plotX, const std::vector<float>& plotY) const
{
    std::vector<unsigned char> grid(nrBlockWidth*nrBlockDepth);
    std::vector<unsigned char> nextGrid(nrBlockWidth*nrBlockDepth);
    for (unsigned char& c : grid)
        c = rand()%2;

    for (unsigned int n = 0 ; n < nrIteration ; n++) {
        for(int j = 0 ; j < nrBlockDepth ; j++) { // Z
            for(int i = 0 ; i < nrBlockWidth ; i++) { // X
                unsigned int countNeighbor = 0;
                for (int a = -1 ; a <= 1 ; a++) {
                    for (int b = -1 ; b <= 1 ; b++) {
                        if (j+b < 0 || j+b >= nrBlockDepth || i+a < 0 || i+a >= nrBlockWidth)
                            continue;

                        if (grid[(j+b)*nrBlockWidth + i+a])
                            countNeighbor++;
                    }
                }
                nextGrid[j*nrBlockWidth+i] = countNeighbor > threshold;
            }
        }
        std::swap(grid, nextGrid);
    }

    UseSplineForHeight(grid, nrBlockWidth, nrBlockDepth, maxBlockHeight, plotX, plotY);    
    stbi_write_png("../data/heightmap/cave.png", nrBlockWidth, nrBlockDepth, 1, grid.data(), nrBlockWidth);    
}