#version 460 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D atlas;

void main()
{    
    FragColor = texture(atlas, texCoords);
}