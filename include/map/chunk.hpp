#pragma once

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "map/voxel.hpp"
#include "graphic/shader.hpp"

#define CHUNK_SIZE 32 // TODO : 32

class Chunk // 32x32x32
{ 
    private:
        glm::ivec3 m_terrainPosition; // Position Column/Row/Depth 
        glm::vec3 m_originPosition; // Back-bottom-left position
        std::vector<Voxel> m_voxels;

        std::vector<glm::vec3> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<unsigned int> m_blockIds;
        std::map<std::string, Face*> m_chunkFaces;
        const unsigned int m_blockId; // TODO : Remove

        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;
        GLuint m_textureId_SBBO;

        Shader m_shader;
        GLuint m_textureId; // TODO : Will be removed and be in TerrainController instead

        void AddFaceIndices(const unsigned int offset);

        void BuildFace(const std::string& faceId, Face* face);
        void BuildFaces();
    
    public:
        Chunk(const std::string& vertexPath, const std::string& fragmentPath, const glm::ivec3 terrainPosition, const glm::vec3 originPosition, const unsigned int blockId); // TODO : Remove blockId 
        // Chunk(glm::vec3 position, bool referenceChunk); // Used in editor mode only
        ~Chunk();
        
        void BuildFullChunk();
        void BuildFlatChunk();
        void BuildWaveChunk(const float frequency);
        void BuildEditorChunk();
    
        void VoxelComputeData();
        void VoxelBufferData();
        void Load();
        
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;
        
        // void addFace(Voxel* v_bottom,int orientation);
        // void removeFace(Voxel* v_bottom,int orientation);
        // void removeFaces(std::string racine_face_id);
        // void buildFace(std::string unique_id_face, bool cond1,int a1, int dec, int a2, int voxel_id, int8_t v1, int8_t v2, int8_t v3, std::vector<glm::vec3> voxel_vertices);
        // void loadChunk(TerrainControler* tc = nullptr);
        // void drawChunk();
        // std::vector<Voxel*> getListeVoxels();
        // void setListeVoxels(std::vector<Voxel*> newListeVoxels);
        // glm::vec3 getPosition();

        // void sendVoxelMapToShader();
};
