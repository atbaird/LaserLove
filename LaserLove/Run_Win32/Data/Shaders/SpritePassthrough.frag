#version 410 core //necessary for all shaders

uniform sampler2D gDiffuseTex;
in vec2 passUV;
in vec4 passColor;

out vec4 outColor;

vec4 UVtoColor(vec2 uv)
{
	return vec4(uv, 0.0f, 1.0f);
}

void main(void)
{
	vec4 c = texture(gDiffuseTex, passUV);
	outColor = c * passColor;
	//outColor = UVtoColor(passUV);
}