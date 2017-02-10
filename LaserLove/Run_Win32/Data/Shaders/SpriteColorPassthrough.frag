#version 410 core //necessary for all shaders

in vec2 passUV;
in vec4 passColor;

out vec4 outColor;

vec4 UVtoColor(vec2 uv)
{
	return vec4(uv, 0.0f, 1.0f);
}

void main(void)
{
	outColor = passColor;
	//outColor = UVtoColor(passUV);
}