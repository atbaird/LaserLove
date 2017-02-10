#version 410 core //necessary for all shaders

uniform sampler2D gTexDiffuse;
uniform sampler2D gDepthTex;

in vec2 passUV0;

out vec4 outColor;

void main()
{
	vec3 color = texture(gTexDiffuse, passUV0).rgb;
	outColor = vec4(color.xyz, 1.f);
}