#version 460 core

in vec2 texCoords;
in float vertexShadow;

out vec4 FragColor;

uniform sampler2D atlas;

void main()
{    
    vec3 color = texture(atlas, texCoords).rgb;
    FragColor = vec4(vertexShadow*color, 1.0);
}