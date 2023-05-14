#version 330 core

out vec4 FragColor;

//in vec3 Color;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main(){             
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    FragColor = texColor;
    //FragColor = vec4(vec3(1.0),1.0);
    //FragColor = Color;
}