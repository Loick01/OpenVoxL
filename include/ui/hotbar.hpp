#pragma once

#include <array>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/key_callback.hpp"
#include "graphic/drawable.hpp"

#define BLOCK_ID_MAX 35

class Hotbar : public EventKeyCallback, public Drawable2D
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
        void Next();
        void Previous();

        void KeyCallback(const std::array<bool, GLFW_KEY_LAST+1>& keys) override;
        void Draw() const override;
};