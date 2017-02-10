#version 410 core //necessary for all shaders

uniform sampler2D gDiffuseTex;
in vec2 passPosition;
in vec2 passUV;
in vec4 passColor;

out vec4 outColor;

vec4 UVtoColor(vec2 uv)
{
	return vec4(uv, 0.0f, 1.0f);
}
vec4 rainbowColor(vec2 pos)
{
	vec2 poscl = pos;
	while(poscl.x > 1.f)
	{
		poscl.x = poscl.x - 1.f;
	}
	while(poscl.x < 0.f)
	{
		poscl.x = poscl.x + 1.f;
	}
	while(poscl.y > 1.f)
	{
		poscl.y = poscl.y - 1.f;
	}
	while(poscl.y < 0.f)
	{
		poscl.y = poscl.y + 1.f;
	}
	vec3 color = vec3(0.5f,cos(poscl.x * 10.f), sin(poscl.y * 10.f));
	return vec4(color, 1.f);

}

void main(void)
{
	vec4 c = texture(gDiffuseTex, passUV);
	outColor = c * passColor * rainbowColor(passPosition);
	//outColor = UVtoColor(passUV);
}