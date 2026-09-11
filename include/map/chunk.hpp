#pragma once

#include <map>
#include <variant>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "core/FastNoise.h" // FastNoise::NoiseType
#include "map/voxel.hpp"
#include "graphic/shader.hpp"

#define CHUNK_SIZE 32

class FastNoise;

enum class ChunkNeighbor // Should not be only used for Chunk ?
{
    Bottom, Top, Back, Front, Left, Right
};

enum class ChunkType
{
    Full, Flat, Wave, Editor, Heightmap, Cheese //, Cave
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
    const unsigned char* heightmap;
    int heightmapWidth;
    int heightmapDepth;
    int channel;

    float frequency;
    int octaves;
    int seed;
    FastNoise::NoiseType noiseType;

    // TODO : Rename
    std::vector<float> plotX;
    std::vector<float> plotY;
    bool useSpline;
};

struct DataCheeseChunk
{
    const FastNoise* noise; 
    float frequency; // [0.5, 10]
    float threshold; // [-1, 1]
};

using DataChunk = std::variant<
    DataFullChunk,
    DataFlatChunk,
    DataWaveChunk,
    DataEditorChunk,
    DataHeightmapChunk,
    DataCheeseChunk
>; 

class Chunk // 32x32x32
{ 
    private:
        glm::ivec3 m_terrainPosition; // Position Column/Row/Depth 
        glm::ivec3 m_terrainSize;
        glm::vec3 m_originPosition; // Back-bottom-left position (m_originPosition = m_terrainPosition*CHUNKSIZE)
        std::vector<Voxel> m_voxels;
        std::vector<Voxel*> m_gridVoxel;

        std::vector<glm::vec3> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<unsigned int> m_blockIds;
        std::vector<unsigned int> m_faceOrientations;
        std::map<std::string, Face*> m_chunkFaces;
        std::map<ChunkNeighbor, const Chunk*> m_chunkNeighbors;
        const unsigned int m_blockId; // TODO : Remove

        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;
        GLuint m_blockId_SSBO;
        GLuint m_faceOrientation_SSBO;

        Shader m_shader;
        GLuint m_textureId; // TODO : Will be removed and be in TerrainController instead

        unsigned int GetBlockIndexInGrid(const glm::ivec3& blockPosition) const;
        void AddVoxel(const glm::vec3 blockPosition, const unsigned int blockId);
        void AddFaceIndices(const unsigned int offset);
        void BuildFace(const std::string& faceId, Face* face);
        void BuildFaces();
    
    public:
        Chunk(const std::string& vertexPath, const std::string& fragmentPath, const glm::ivec3 terrainPosition, const glm::ivec3 terrainSize,
            const glm::vec3 originPosition, const unsigned int blockId); // TODO : Remove blockId 
        // Chunk(glm::vec3 position, bool referenceChunk); // Used in editor mode only
        ~Chunk();
        
        const Chunk* GetChunkNeighbor(const ChunkNeighbor which) const;
        void SetChunkNeighbor(const ChunkNeighbor which, const Chunk* neighbor);
        
        glm::ivec3 GetTerrainPosition() const;

        bool IsEmptyAt(const unsigned voxelIndex) const;

        void Build(const DataFullChunk& data);
        void Build(const DataFlatChunk& data);
        void Build(const DataWaveChunk& data);
        void Build(const DataEditorChunk& data);
        void Build(const DataHeightmapChunk& data);
        void Build(const DataCheeseChunk& data);
    
        void VoxelComputeData();
        void VoxelBufferData();
        void Load();
        
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
};
