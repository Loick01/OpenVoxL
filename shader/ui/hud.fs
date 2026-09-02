#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hudTexture;

void main(){
    vec4 color = texture(hudTexture, TexCoords);
    // TODO : Remove ?
    // if (color.a < 0.1){ 
    //     discard;
    // }
    FragColor = color;
}
