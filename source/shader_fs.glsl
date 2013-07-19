#version 150

out vec4 fragment_color;

uniform vec4 OutColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

void main()
{
	fragment_color = OutColor;
}