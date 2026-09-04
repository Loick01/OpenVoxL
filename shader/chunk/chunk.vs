#version 460 core

layout (location = 0) in vec3 aPos;

layout(std430, binding = 0) readonly buffer BlockIds
{
    uint textureIds[];
};

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec2 atlasTexCoords[4] = vec2[4](
        vec2(0.0, 0.1),
        vec2(0.2, 0.1),
        vec2(0.0, 0.0),
        vec2(0.2, 0.0)
);

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    uint blockId = textureIds[gl_VertexID/36];
    texCoords = atlasTexCoords[gl_VertexID%4];
    texCoords[0] += blockId%5*0.2;
    texCoords[1] += blockId/5*0.1; 
};
