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
        unsigned int m_selectorPosition;
        GLuint m_blockId_SSBO;

        void UpdateBlockIds();

    public:
        Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
        ~Hotbar();

        unsigned int GetSelectedBlockId() const;
        void SelectorNext();
        void SelectorPrevious();

        void ScrollCallback(const double xoffset, const double yoffset);
        void Draw() const override;
};