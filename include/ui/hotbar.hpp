#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"
#include "ui/quad.hpp"

class Hotbar : public Drawable
{
    private:
        std::vector<Quad> m_hotbarQuads;

        unsigned int m_countIndex;

        bool m_show;

    public:
        Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int hotbarWidth);

        void LoadTexture() override;
        void Load() override;
        void Draw() const override;
};