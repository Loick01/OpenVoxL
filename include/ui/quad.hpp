#pragma once

#include <vector>

struct Quad
{
    std::vector<glm::vec3> vertices; // z = 0
    std::vector<unsigned int> indices;

    Quad(const glm::vec2 origin, const float width, const float height) { // Origin is the bottom left point
        vertices.push_back(glm::vec3(origin, 0.f));
        vertices.push_back(glm::vec3(origin[0]+width, origin[1], 0.f));
        vertices.push_back(glm::vec3(origin[0], origin[1]+height, 0.f));
        vertices.push_back(glm::vec3(origin[0]+width, origin[1]+height, 0.f));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(3);
    }
};