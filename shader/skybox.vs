#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    // vec4 pos = projection * view * vec4(aPos, 1.0);
    // gl_Position = pos.xyww;
    // TexCoords = aPos; // Because the cube is centered, its position vector is also a direction vector from the origin
    
    TexCoords = aPos;
    gl_Position = vec4(aPos, 1.0);
};
