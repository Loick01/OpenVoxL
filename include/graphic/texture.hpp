#pragma once

#include <glad/glad.h>
#include <array>
#include <stdexcept>

#include "graphic/stb_image.h"

GLuint LoadTexture2D(const std::string& pathTexture);
GLuint LoadCubemapTexture(const std::array<std::string,6>& pathTextures);