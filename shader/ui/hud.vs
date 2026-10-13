#version 460 core

layout(location = 0) in vec2 aPos;

out vec2 TexCoords;

uniform int windowWidth;
uniform int windowHeight;
uniform int handBlockIndex;

vec2 hudTexCoords[20] = vec2[20](
    // Hotbar
    vec2(0.001953,0.467391),
    vec2(0.708984,0.467391),
    vec2(0.001953,0.0108695),
    vec2(0.708984,0.0108695),
    // Selector
    vec2(0.001953,0.994565),
    vec2(0.091796,0.994565),
    vec2(0.001953,0.494565),
    vec2(0.091796,0.494565),
    // Cursor
    vec2(0.949218,0.260869),
    vec2(0.984375,0.260869),
    vec2(0.949218,0.065217),
    vec2(0.984375,0.065217),
    // Health
    vec2(0.097656,0.630434),
    vec2(0.803711,0.630434),
    vec2(0.097656,0.543478),
    vec2(0.803711,0.543478),
    // Stamina
    vec2(0.097656,0.630434),
    vec2(0.803711,0.630434),
    vec2(0.097656,0.543478),
    vec2(0.803711,0.543478)
);

void main() {
    vec2 screenCoordinates = aPos;
    // screenCoordinates[0] += handBlockIndex * 80.0;
    vec2 normalizedPosition = vec2(screenCoordinates[0]/windowWidth, screenCoordinates[1]/windowHeight);
    gl_Position = vec4(normalizedPosition*2.0-1.0, 0.0, 1.0);
    TexCoords = hudTexCoords[gl_VertexID];
}
