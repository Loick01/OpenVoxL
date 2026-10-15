#version 460 core

layout(location = 0) in vec2 aPos;

layout(std430, binding = 0) readonly buffer BlockIds
{
    uint textureIds[];
};

out vec2 TexCoords;

uniform int windowWidth;
uniform int windowHeight;

vec2 atlasTexCoords[4] = vec2[4](
        vec2(0.0, 0.1),
        vec2(0.2, 0.1),
        vec2(0.0, 0.0),
        vec2(0.2, 0.0)
);

void main() {
    vec2 screenCoordinates = aPos;
    vec2 normalizedPosition = vec2(screenCoordinates[0]/windowWidth, screenCoordinates[1]/windowHeight);
    gl_Position = vec4(normalizedPosition*2.0-1.0, 0.0, 1.0);
    uint blockId = textureIds[gl_VertexID/4];
    TexCoords = atlasTexCoords[gl_VertexID%4];
    TexCoords[0] += blockId%5*0.2;
    TexCoords[1] += blockId/5*0.1;
}
