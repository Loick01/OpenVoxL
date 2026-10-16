#pragma once

#include <array>

struct Quad
{
    std::array<glm::vec2, 4> vertices;
    std::array<unsigned int, 6> indices;

    Quad(const glm::vec2 origin, const float width, const float height) { // Origin is the bottom left point
        vertices[0] = origin;
        vertices[1] = glm::vec2(origin[0]+width, origin[1]);
        vertices[2] = glm::vec2(origin[0], origin[1]+height);
        vertices[3] = glm::vec2(origin[0]+width, origin[1]+height);

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 2;
        indices[4] = 1;
        indices[5] = 3;
    }

    glm::vec2 GetOrigin() const {
        return vertices[0];
    }

    void Translate(const glm::vec2 t) {
        for (glm::vec2& v : vertices)
            v += t;
    }

    void SetPosition(const glm::vec2 p) {
        const glm::vec2 t = p - vertices[0];
        Translate(t);
    }
};