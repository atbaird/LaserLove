#version 410 core

uniform vec4 gColor; //constant color
uniform sampler2D gDiffuseTex;

in vec3 passPosition;
in vec4 passColor;
in vec2 passUV0;
in vec3 passTangent;
in vec3 passBitangent;
in vec3 passNormal;

out vec4 outColor;

void main(void)
{
	//vec4 diffuse = texture(gDiffuseTex, passUV0);
	//outColor = diffuse;
	//outColor = passColor * gColor * diffuse;
	//outColor = vec4(passUV0, 0.f, 1.f);

	outColor = passColor; //passthrough only.
}