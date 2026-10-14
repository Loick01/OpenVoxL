#version 460 core

layout(location = 0) in vec2 aPos;

uniform int windowWidth;
uniform int windowHeight;

void main() {
    vec2 screenCoordinates = aPos;
    vec2 normalizedPosition = vec2(screenCoordinates[0]/windowWidth, screenCoordinates[1]/windowHeight);
    gl_Position = vec4(normalizedPosition*2.0-1.0, 0.0, 1.0);
}
