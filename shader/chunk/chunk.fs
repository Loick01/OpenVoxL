#version 460 core

flat in int indexBlock; // Remove

out vec4 FragColor;

vec3 blockColors[3] = vec3[3] // Remove
(
    vec3(0.9, 0.2, 0.1),
    vec3(0.1, 0.9, 0.2),
    vec3(0.2, 0.1, 0.9)
);

void main()
{    
    FragColor = vec4(blockColors[indexBlock%3], 1.0);
}