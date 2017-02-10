#version 410 core //necessary for all shaders
//this is only used to pass through data.
//(Program) -> vert -> Frag -> screen

//Renderer uses normalized device coordinates.

//white triangle example from class points: (-0.5, -0.5, 0) (0.5, -0.5, 0) (0.5, 0.5, 1)

uniform mat4 gModel;
uniform mat4 gView;
uniform mat4 gProj;

in vec2 inPosition;
in vec4 inColor;
in vec2 inUV0;

out vec4 passColor;
out vec2 passUV0;

void main( void )
{
	passColor = inColor;
	passUV0 = inUV0; //for use with textures

	vec4 pos = vec4(inPosition, 0.0f, 1.0f);
	pos =pos * gModel * gView * gProj; // row major
	// pos = pos * gModel * gView * gProj; //column major
	
	gl_Position = pos;
}