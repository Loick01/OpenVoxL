#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;

void main(){
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    vec4 color = texture(text, TexCoords);
    FragColor = color;
}
