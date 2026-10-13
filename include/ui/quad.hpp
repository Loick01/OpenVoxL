#pragma once

#include <vector>

struct Quad
{
    std::vector<glm::vec2> vertices;
    std::vector<unsigned int> indices;

    Quad(const glm::vec2 origin, const float width, const float height) { // Origin is the bottom left point
        vertices.push_back(origin);
        vertices.push_back(glm::vec2(origin[0]+width, origin[1]));
        vertices.push_back(glm::vec2(origin[0], origin[1]+height));
        vertices.push_back(glm::vec2(origin[0]+width, origin[1]+height));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(3);
    }
};