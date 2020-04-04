#version 460 core
layout(location = 0) out vec4 FragColor;

in vec4 v_TexColor;

//uniform vec4 u_color;

void main()

{
	FragColor = v_TexColor;
}