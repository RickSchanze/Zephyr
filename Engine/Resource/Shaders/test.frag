#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 Pos;

uniform sampler2D texture_diffuse0;

void main()
{
    FragColor = texture(texture_diffuse0, TexCoords);
    //   FragColor = Pos;
}
