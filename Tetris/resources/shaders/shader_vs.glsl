#version 330 core
layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aColor;
layout (location = 2) in vec2 aTexCoords;

//out vec3 Color;
out vec2 TexCoords;

uniform mat4 model;
//uniform mat4 view;
uniform mat4 projection;

void main(){
    //Color = aColor;
    TexCoords = aTexCoords;
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * model * vec4(aPos, 1.0, 1.0);
}