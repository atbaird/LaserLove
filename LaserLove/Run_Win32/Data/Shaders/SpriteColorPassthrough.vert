#version 410 core //necessary for all shaders

uniform mat4 gProj; //Ortho
uniform mat4 gView; //no model

in vec2 inPosition;
in vec2 inUV0;
in vec4 inColor;

out vec2 passUV;
out vec4 passColor;

vec4 vec2ToVec4(vec2 vec)
{
	return vec4(vec, 0.0f, 1.0f);
}
vec4 vec3ToVec4(vec3 vec)
{
	return vec4(vec, 1.0f);
}

void main(void)
{
	passUV = inUV0;
	passColor = inColor;
	//vec4 p = vec2ToVec4(inPosition);
	
	vec4 pos = vec4(inPosition, 0.0f, 1.0f);
	pos = pos * gView * gProj;

	gl_Position = pos;
}
