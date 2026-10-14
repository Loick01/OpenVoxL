#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/drawable.hpp"

class Hotbar : public Drawable2D
{
    private:
        std::vector<Quad> m_hotbarQuads;

    public:
        Hotbar(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int windowWidth, const unsigned int windowHeight);
};