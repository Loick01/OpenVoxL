#pragma once

#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"

class Hotbar : public Drawable2D
{
    private:
        std::vector<Quad> m_hotbarQuads;

        std::array<unsigned int, 9> m_blockIds;
        GLuint m_blockId_SSBO;

        void UpdateBlockIds();

    public:
        Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
        ~Hotbar();

        unsigned int GetSelectedBlockId(const unsigned int index) const;

        void Draw() const override;
};