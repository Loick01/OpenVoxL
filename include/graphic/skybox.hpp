#pragma once

#include "graphic/drawable.hpp"

class Skybox : public Drawable3D {
    public:
        Skybox(const std::string& vertexPath, const std::string& fragmentPath);
        
        void Load() override;
        void Draw(const glm::mat4& projection, const glm::mat4& view) const override;
};