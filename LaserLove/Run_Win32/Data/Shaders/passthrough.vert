#version 410 core //necessary for all shaders
//this is only used to pass through data.
//(Program) -> vert -> Frag -> screen

//Renderer uses normalized device coordinates.

//white triangle example from class points: (-0.5, -0.5, 0) (0.5, -0.5, 0) (0.5, 0.5, 1)

uniform mat4 gModel;
uniform mat4 gView;
uniform mat4 gProj;

in vec3 inPosition;
in vec4 inColor;
in vec2 inUV0;
in vec3 inTangent;
in vec3 inBitangent;
in vec3 inNormal;

out vec3 passTangent;
out vec3 passBitangent;
out vec3 passNormal;
out vec4 passColor;
out vec2 passUV0;
out vec3 passPosition;

void main( void )
{
	passTangent = (vec4(inTangent, 0.f) * gModel).xyz;
	passBitangent = (vec4(inBitangent, 0.f) * gModel).xyz;
	passNormal = (vec4(inNormal, 0.f) * gModel).xyz;
	passColor = inColor;
	passUV0 = inUV0; //for use with textures

	vec4 pos = vec4(inPosition, 1.0f);
	passPosition = (pos * gModel).xyz;
	pos =pos * gModel * gView * gProj; // row major
	// pos = pos * gModel * gView * gProj; //column major
	
	gl_Position = pos;
}