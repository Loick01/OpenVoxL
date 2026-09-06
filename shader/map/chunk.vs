#version 460 core

layout (location = 0) in vec3 aPos;

layout(std430, binding = 0) readonly buffer BlockIds
{
    uint textureIds[];
};

out vec2 texCoords;
out float vertexShadow;

uniform mat4 projection;
uniform mat4 view;

vec2 atlasTexCoords[4] = vec2[4](
        vec2(0.0, 0.1),
        vec2(0.2, 0.1),
        vec2(0.0, 0.0),
        vec2(0.2, 0.0)
);

float blockShadow[24] = float[24](
        0.2,0.2,0.2,0.2, // Bottom
        1.2,1.2,1.2,1.2, // Top
        0.8,0.8,0.8,0.8, // Back
        0.8,0.8,0.8,0.8, // Front
        0.5,0.5,0.5,0.5, // Left
        0.5,0.5,0.5,0.5 // Right
);

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    uint blockId = textureIds[gl_VertexID/4];
    texCoords = atlasTexCoords[gl_VertexID%4];
    texCoords[0] += blockId%5*0.2;
    texCoords[1] += blockId/5*0.1;
    vertexShadow = blockShadow[gl_VertexID%24]; // TODO : I need faceId SSBO
};
