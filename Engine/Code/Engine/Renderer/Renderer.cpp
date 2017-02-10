#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT_Skeleton.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/2D/SpriteVert.hpp"
#include "Engine/2D/Particle2D.hpp"

Renderer* g_Renderer = nullptr;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Common Shader Binds
const std::string Renderer::s_inColor = "inColor";
const std::string Renderer::s_inPos = "inPosition";
const std::string Renderer::s_inBoneIndices = "inBoneIndices";
const std::string Renderer::s_inBoneWeights = "inBoneWeights";
const std::string Renderer::s_inUV0 = "inUV0";
const std::string Renderer::s_inTangent = "inTangent";
const std::string Renderer::s_inBitangent = "inBitangent";
const std::string Renderer::s_inNormal = "inNormal";
const std::string Renderer::s_postProcessDiffuseBindName = "gTexDiffuse";
const std::string Renderer::s_postProcessDepthBindName = "gTexDepth";

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//default colors
const AABB2 Renderer::s_WholeTextureCoords = AABB2(Vector2(0.f,0.f), Vector2(1.f,1.f));

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Constructors
Renderer::Renderer()
	: m_fbo(nullptr)
{
	loadExternalMethods();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.5f);

	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	ScreenStartAndEnd[0] = IntVector2(m_viewport[0], m_viewport[1]);
	ScreenStartAndEnd[1] = IntVector2(m_viewport[2], m_viewport[3]);
}
Renderer::~Renderer()
{
	lines.clear();
	/*
	for (std::map<std::string, std::map<std::string, Texture*>>::iterator it = s_textureRegistry.begin(); it != s_textureRegistry.end(); it++) {
		if (s_textureRegistry[it] != nullptr)
		{
			delete it->second;
			s_textureRegistry[it] = nullptr;
		}
	}
	*/
}



GLenum GetGLenumForPrimType(const PrimitiveType& drawMode) //unsigned int
{
	GLenum toReturn = GL_POINTS;
	switch (drawMode)
	{
	case PRIMITIVE_POINTS:
		toReturn = GL_POINTS;
		break;
	case PRIMITIVE_LINES:
		toReturn = GL_LINES;
		break;
	case PRIMITIVE_TRIANGLES:
		toReturn = GL_TRIANGLES;
		break;
	case PRIMITIVE_QUADS:
		toReturn = GL_QUADS;
		break;
	case PRIMITIVE_DYNAMIC_DRAW:
		toReturn = GL_DYNAMIC_DRAW;
		break;
	default:
		break;
	}
	return toReturn;
}

void Renderer::loadExternalMethods()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");

	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC)wglGetProcAddress("glDrawArraysExt");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");

	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");

	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");

	glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");

	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)wglGetProcAddress("glFramebufferTexture");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");

	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Render Shape
void Renderer::DrawQuad3D(const Vector3& position0, Vector3& position1, const Vector3& position2, const Vector3& position3, const Rgba& color)
{
	glDisable(GL_TEXTURE_2D);
	std::vector<Vertex_PCT> quad;
	{
		quad.push_back(Vertex_PCT(Vector3(position0.x, position0.y, position0.z), color));
		quad.push_back(Vertex_PCT(Vector3(position1.x, position1.y, position1.z), color));
		quad.push_back(Vertex_PCT(Vector3(position2.x, position2.y, position2.z), color));
		quad.push_back(Vertex_PCT(Vector3(position3.x, position3.y, position3.z), color));
	}
	DrawVertexArray(quad, PRIMITIVE_QUADS, nullptr);
}

void Renderer::DrawTexturedQuad3D(const Vector3& position0, Vector3& position1, const Vector3& position2, const Vector3& position3, Texture* texture, AABB2 texCoords, const Rgba& tint)
{
	std::vector<Vertex_PCT> quad;
	{
		quad.push_back(Vertex_PCT(Vector3(position0.x, position0.y, position0.z), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quad.push_back(Vertex_PCT(Vector3(position1.x, position1.y, position1.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
		quad.push_back(Vertex_PCT(Vector3(position2.x, position2.y, position2.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quad.push_back(Vertex_PCT(Vector3(position3.x, position3.y, position3.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
	}
	DrawVertexArray(quad, PRIMITIVE_QUADS, texture);
}

void Renderer::DrawAABB(const AABB2& bounds, const Rgba& color)
{
	std::vector<Vertex_PCT> quad;
	//draws a square.
	{
		quad.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y), color));
		quad.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y), color));
		quad.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y), color));
		quad.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y), color));
	}
	DrawVertexArray(quad, PRIMITIVE_QUADS, nullptr);
}

void Renderer::DrawTexturedAABB(const AABB2& bounds, const Texture* texture, const AABB2& texCoord, const Rgba& tint)
{
	//draws a textured AABB, aka square,
	std::vector<Vertex_PCT> quad;
	{
		quad.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y), tint, Vector2(texCoord.mins.x, texCoord.maxs.y)));
		quad.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y), tint, Vector2(texCoord.maxs.x, texCoord.maxs.y)));
		quad.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y), tint, Vector2(texCoord.maxs.x, texCoord.mins.y)));
		quad.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y), tint, Vector2(texCoord.mins.x, texCoord.mins.y)));
	}
	DrawVertexArray(quad, PRIMITIVE_QUADS, texture);
	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawLinedBox(const AABB2& bounds, const Rgba& tint, const float& lineThickness)
{
	std::vector<Vertex_PCT> localLines;
	{
		localLines.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y), tint));
		localLines.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y), tint));

		localLines.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y), tint));
		localLines.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y), tint));

		localLines.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y), tint));
		localLines.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y), tint));

		localLines.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y), tint));
		localLines.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y), tint));
	}
	setLineSize(lineThickness);
	DrawVertexArray(localLines, PRIMITIVE_LINES, nullptr);
}

void Renderer::DrawQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Rgba& color)
{
	//Literally just draws a square of color
	std::vector<Vertex_PCT> quad;
	{
		quad.push_back(Vertex_PCT(Vector3(position0.x, position0.y), color));
		quad.push_back(Vertex_PCT(Vector3(position1.x, position1.y), color));
		quad.push_back(Vertex_PCT(Vector3(position2.x, position2.y), color));
		quad.push_back(Vertex_PCT(Vector3(position3.x, position3.y), color));
	}
	DrawVertexArray(quad, PRIMITIVE_QUADS, nullptr);
}

void Renderer::RenderPolygon(const Vector2& center, float radius, int numSides, float degreesOffSet, Rgba color, bool isDotted )
{
	isDotted = isDotted;
	//renders a polygon shape of the given number of sides
	//const float radiansTotal = 2.f * ExtendMath->pi;
	const float radiansTotal = (float)(2.f * PI);
	const float radiansPerSide = radiansTotal / numSides;
	glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
	std::vector<Vertex_PCT> localLines;
	{
		for (float radians = 0.f; radians < radiansTotal; radians += radiansPerSide) {
			//float rotatedRadians = (0.5f * pi) + radians;
			float rotatedRadians = radians + DegreesToRadians(degreesOffSet);
			//float rotatedRadians = radians + DegreesToRadians(degreesOffSet);
			float x = (cos(rotatedRadians) * radius) + center.x;
			float y = (sin(rotatedRadians) * radius) + center.y;
			localLines.push_back(Vertex_PCT(Vector3(x,y, 0.f), color));
		}
	}
	setLineSize(1.f);
	DrawVertexArray(localLines, PRIMITIVE_LINES, nullptr);
}

void Renderer::RenderLine(const Vector2& startPoint, const Vector2& endPoint, Rgba color, float lineThickness)
{
	//draws a straight line from the startPoint to the endPoint
	std::vector<Vertex_PCT> line;
	line.push_back(Vertex_PCT(Vector3(startPoint.x, startPoint.y, 0.f), color));
	line.push_back(Vertex_PCT(Vector3(endPoint.x, endPoint.y, 0.f), color));
	setLineSize(lineThickness);
	DrawVertexArray(line, PRIMITIVE_LINES, nullptr);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Screen
void Renderer::SetScreenStart(const IntVector2& start)
{
	ScreenStartAndEnd[0] = start;
}

void Renderer::SetScreenEnd(const IntVector2& end)
{
	ScreenStartAndEnd[1] = end;
}

const int Renderer::GetScreenWidth() const
{
	return ScreenStartAndEnd[1].x - ScreenStartAndEnd[0].x;
}

const int Renderer::GetScreenHeight() const
{
	return ScreenStartAndEnd[1].y - ScreenStartAndEnd[0].y;
}

const IntVector2 Renderer::GetStartScreen() const
{
	return ScreenStartAndEnd[0];
}
const IntVector2 Renderer::GetEndScreen() const
{
	return ScreenStartAndEnd[1];
}
void Renderer::SwapBuffer() const
{
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Framebuffer
void Renderer::FramebufferBind(FrameBuffer* fbo, const Vector2& scaling)
{
	scaling;
	if (m_fbo == fbo)
	{
		return;
	}
	m_fbo = nullptr;
	m_fbo = fbo;
	if (m_fbo == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
		glViewport(0, 0, g_Renderer->GetScreenWidth(), g_Renderer->GetScreenHeight());
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_handle);
		glViewport(0, 0, fbo->pixel_width, fbo->pixel_height);

		GLenum render_targets[32];
		for (uint32_t i = 0; i < fbo->color_targets.size(); ++i)
		{
			render_targets[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(fbo->color_targets.size(), render_targets);
	}
}
//only used for debug purposes
//You will probably never use it after this lesson (2/23/2016; SD3 Renderring)
void Renderer::FramebufferCopyToBack(const FrameBuffer* fbo) const //Renderer?
{
	if (fbo == nullptr)
	{
		return;
	}
	GLuint fboh = fbo->fbo_handle;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboh);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NULL);

	uint32_t read_width = fbo->pixel_width;
	uint32_t read_height = fbo->pixel_height;

	uint32_t draw_width = g_Renderer->GetScreenWidth();
	uint32_t draw_height = g_Renderer->GetScreenHeight();

	glBlitFramebuffer(0, 0, //lower left corner pixel
		read_width, read_height, //top right corner
		0, 0, //lower left pixel of read buffer
		draw_width, draw_height, //top right pixel of read buffer
		GL_COLOR_BUFFER_BIT,
		GL_NEAREST);
}
void Renderer::RenderPostProcess(FrameBuffer* fbo, Material* myMaterial, const Mesh* myMesh) const
{
	Matrix44 model, view, projection;
	model.MatrixMakeIdentity();
	view.MatrixMakeIdentity();
	projection.MatrixMakeIdentity();
	myMaterial->SetMatrices(model, view, projection);
	myMaterial->SetTexture(fbo->get_color_texture(0), 0, s_postProcessDiffuseBindName, 0);
	myMaterial->SetTexture(fbo->get_depth_stencil_target(), 1, s_postProcessDepthBindName, 1);


	myMesh->DrawWith(myMaterial);
	
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Sampler Related

void Renderer::BindSampler(const unsigned int& samplerID, const unsigned int& units) const
{
	glBindSampler(units, samplerID);
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Text related
void Renderer::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint, const BitmapFont* font, const float& cellAspect)
{
	if (!font)
		return;
	CullFaceTrueFalse(false);
	int numTextChars = (int)asciiText.size();
	Vector2 cellSize = Vector2(cellAspect * cellHeight, cellHeight);
	Vector2 shiftRight = Vector2(cellSize.x, 0.f);
	
	const Texture* texture = font->GetTexture();

	Vector2 currentPosition = startBottomLeft;
	for (int i = 0; i < numTextChars; i++)
	{
		//Vector2 positionMaxes = Vector2(currentPosition.x, currentPosition.y + cellSize.y);
		int val = (int)asciiText[i];
		AABB2 fontCoords = font->GetTexCoordsForGlyph(val);
		DrawTexturedAABB(AABB2(currentPosition, Vector2(currentPosition.x + cellSize.x, currentPosition.y + cellSize.y)), texture, fontCoords, tint);
		currentPosition += shiftRight;
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Shaders Specific
void Renderer::BindTextureToProgram(const unsigned int& samplerID, const unsigned int& bindLoc, const unsigned int& textureID) const
{
	glActiveTexture(GL_TEXTURE0 + bindLoc);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindSampler(bindLoc, samplerID);
}
const bool Renderer::RunShaderProgram(const unsigned int& vaoID, const unsigned int& shaderProgID, const unsigned int& numberOfElemToDraw,
	const PrimitiveType& primitiveType, const DataType& IBODataType) const
{
	unsigned int glPrimType = GetGLenumForPrimType(primitiveType);
	unsigned int glDataType = DeteremineDataType(IBODataType);
	glBindVertexArray(vaoID);
	glUseProgram(shaderProgID);
	glDrawElements(glPrimType, numberOfElemToDraw, glDataType, (GLvoid*)0);
	glUseProgram(NULL);
	glBindVertexArray(NULL);
	return true;
}
const unsigned int Renderer::DetermineIfFragOrVertShader(const std::string& fileLocation) const
{
	std::string extension = fileLocation.substr(fileLocation.length() - 5, 5);
	if (extension == ".vert")
	{
		return GL_VERTEX_SHADER;
	}
	else if (extension == ".frag")
	{
		return GL_FRAGMENT_SHADER;
	}
	return 0;
}

const unsigned int Renderer::CreateShader(const unsigned int& shaderType)
{
	GLuint shader_id = glCreateShader(shaderType);
	return shader_id;
}

const unsigned int Renderer::CompileShader(const std::string& fileLocation, bool& loadedCorrectly)
{
	unsigned int shader_type = g_Renderer->DetermineIfFragOrVertShader(fileLocation);
	std::vector<unsigned char> buffer;

	FileUtils::LoadBinaryFileToBuffer(fileLocation, buffer); //static bool LoadTextFileToBuffer(const std::string& filepath, std::string& out_buffer);
	buffer.push_back(0);
	unsigned int shader_id = CreateShader(shader_type);
	ASSERT_OR_DIE(shader_id != NULL, "Shader did not get created correctly.");

	int src_length = buffer.size();
	GLchar* bufferStart = (GLchar*)& buffer[0];
	glShaderSource(shader_id, 1, &bufferStart, &src_length);

	glCompileShader(shader_id);

	//check for GLSL Shader Compile Errors
	GLint status = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		//prep error report.
		GLint length = 0;
		GLint glversionLength = 0;
		GLint glslVersionLength = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		std::string log;
		char* bufferN = new char[length + 1];
		glGetShaderInfoLog(shader_id, length, &length, bufferN);
		glGetShaderiv(shader_id, GL_VERSION, &glversionLength);
		char* bufferV = new char[glversionLength + 1];
		glGetShaderInfoLog(shader_id, glversionLength, &glversionLength, bufferV);
		glGetShaderiv(shader_id, GL_SHADING_LANGUAGE_VERSION, &glslVersionLength);
		char* buffergl = new char[glversionLength + 1];
		glGetShaderInfoLog(shader_id, glslVersionLength, &glslVersionLength, buffergl);
		bufferN[length] = NULL;
		//----
		//copy report to string.
		std::string toPrint = "glsl Version ";
		for (size_t i = 0; i < (size_t)(glslVersionLength + 1); i++)
		{
			toPrint += buffergl[i];
		}
		toPrint += " , gl Version ";
		for (size_t i = 0; i < (size_t)(glversionLength + 1); i++)
		{
			toPrint += bufferV[i];
		}
		toPrint += " : ";
		for (size_t i = 0; i < (size_t)(length + 1); i++)
		{
			toPrint += bufferN[i];
		}
		//----
		//clean up after self
		//----
		//print report, and die.
		toPrint = fileLocation + " : " + toPrint;
		//g_Console->ConsolePrint(toPrint, Rgba(1.f, 0.f, 0.f, 1.f));
		loadedCorrectly = false;
		delete bufferN;
		delete bufferV;
		delete buffergl;
		glDeleteShader(shader_id);
		ERROR_RECOVERABLE(toPrint);
		return 0;
	}
	loadedCorrectly = true;
	return shader_id;
}

void Renderer::DeleteShader(const unsigned int& shaderID)
{
	glDeleteShader(shaderID);
}


const unsigned int Renderer::CreateAndLinkShaderProgram(const unsigned int& vertexShaderID, const unsigned int& fragmentShaderID)
{
	GLuint program_id = glCreateProgram();
	ASSERT_OR_DIE(program_id != NULL, "NULL SHADER!");

	glAttachShader(program_id, (GLuint)vertexShaderID);
	glAttachShader(program_id, (GLuint)fragmentShaderID);

	glLinkProgram(program_id);

	GLint status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) //aka: status == 0
	{
		GLint log_length;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

		std::string buffer;
		buffer.resize(log_length);

		glGetProgramInfoLog(program_id, buffer.length(), &log_length, (GLchar*)buffer.data());
		glDeleteProgram(program_id);
		ASSERT_OR_DIE(false, buffer);
		return NULL;
	}
	else
	{
		glDetachShader(program_id, (GLuint)vertexShaderID);
		glDetachShader(program_id, (GLuint)fragmentShaderID);
	}
	return program_id;
}
void Renderer::DeleteShaderProgram(const unsigned int& shaderProgramID)
{
	glDeleteProgram(shaderProgramID);
}

void Renderer::SetUseShaderProgram(const unsigned int& programId)
{
	glUseProgram(programId);
}
void Renderer::DeactivateShaderProgram()
{
	glUseProgram(NULL);
}

const int Renderer::GetUniformLocationFromShaderProgramByName(const unsigned int& programID, const std::string& variableName) const
{
	GLint loc = glGetUniformLocation(programID, variableName.c_str());
	return loc;
}

void Renderer::SetVariableLocToValueFourFloatPointer(const int& Value, const Vector4* toSet, const int& numberOfValues) const
{
	glUniform4fv(Value, numberOfValues, (const float*)toSet);
}

void Renderer::SetVariableLocToValueThreeFloatPointer(const int& Value, const Vector3* toSet, const int& numberOfValues) const
{
	glUniform3fv(Value, numberOfValues, (const float*)toSet);
}

void Renderer::SetVariableLocToValueTwoFloatPointer(const int& Value, const Vector2* toSet, const int& numberOfValues) const
{
	glUniform2fv(Value, numberOfValues, (const float*)toSet);
}
void Renderer::SetVariableLocToValueOneFloatPointer(const int& Value, const float* toSet, const int& numberOfValues) const
{
	glUniform1fv(Value, numberOfValues, toSet);
}

void Renderer::SetVariableLocToValueOneIntPointer(const int& Value, const int* toSet, const int& numberOfValues) const
{
	glUniform1iv(Value, numberOfValues, toSet);
}
void Renderer::SetVariablesLocToValue4By4Matrix(const int& Value, const Matrix44* toSet, const bool& transpose, const int& numberOfValues) const
{
	glUniformMatrix4fv(Value, numberOfValues, transpose, (const float*)toSet);
}

const unsigned int Renderer::CreateSampler(const DistanceType& min_tex_filter, const DistanceType& mag_tex_filter, const DistanceType& u_wrap, const DistanceType& v_wrap)
{
	unsigned int minFilter = DetermineDistanceType(min_tex_filter);
	unsigned int maxFilter = DetermineDistanceType(mag_tex_filter);
	unsigned int uWrap = DetermineDistanceType(u_wrap);
	unsigned int vWrap = DetermineDistanceType(v_wrap);
	unsigned int id = 0;
	glGenSamplers(1, &id);

	glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, minFilter);
	glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, maxFilter);
	glSamplerParameteri(id, GL_TEXTURE_WRAP_S, uWrap);
	glSamplerParameteri(id, GL_TEXTURE_WRAP_T, vWrap);
	glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return id;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//VAOs
void Renderer::GenerateVAO(unsigned int& vaoIDGenerated)
{
	if (vaoIDGenerated == 0 && glGenVertexArrays != nullptr)
	{
		glGenVertexArrays(1, &vaoIDGenerated);
	}
}

const unsigned int Renderer::DeteremineDataType(const DataType& dataType) const
{

	if (dataType == BYTE_)
	{
		return GL_BYTE;
	}
	else if (dataType == UNSIGNED_BYTE_)
	{
		return GL_UNSIGNED_BYTE;
	}
	else if (dataType == SHORT_)
	{
		return GL_SHORT;
	}
	else if (dataType == UNSIGNED_SHORT_)
	{
		return GL_UNSIGNED_SHORT;
	}
	else if (dataType == INTEGER_)
	{
		return GL_INT;
	}
	else if (dataType == UNSIGNED_INTEGER_)
	{
		return GL_UNSIGNED_INT;
	}
	else if (dataType == FLOATING_POINT_)
	{
		return GL_FLOAT;
	}
	else if (dataType == TWO_BYTES_)
	{
		return GL_2_BYTES;
	}
	else if (dataType == THREE_BYTES_)
	{
		return GL_3_BYTES;
	}
	else if (dataType == FOUR_BYTES_)
	{
		return GL_4_BYTES;
	}
	else if (dataType == DOUBLE_)
	{
		return GL_DOUBLE;
	}

	return 0;
}

const unsigned int Renderer::DetermineDistanceType(const DistanceType& distanceType) const
{
	if (distanceType == NEAREST_DISTANCE_TYPE)
	{
		return GL_NEAREST;
	}
	else if (distanceType == LINEAR_DISTANCE_TYPE)
	{
		return GL_LINEAR;
	}
	else if (distanceType == REPEAT_DISTANCE_TYPE)
	{
		return GL_REPEAT;
	}
	return 0;
}


void Renderer::ShaderProgramBindProperty(const unsigned int& program,
	const std::string& name,
	const int& count,
	const bool& normalize,
	const int& stride,
	const int& offset,
	const DataType& type)
{
	unsigned int trueType = DeteremineDataType(type);

	GLint pos_bind = glGetAttribLocation(program, name.c_str());
	if (pos_bind >= 0) {
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, // Bind Point to Shader 
			count,                // Number of data elements passed
			trueType,         // type of data 
			normalize,         // normalize the data
			stride, // stride
			(GLvoid*)offset); // offset of member
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//BindMeshToVAOs
void Renderer::BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID,
	const unsigned int& shaderProgramID, const Vertex_PCT&)
{
	//Vertex_PCT version
	glBindVertexArray(vaoID);
	const unsigned int sizeOfVert = sizeof(Vertex_PCT);

	GLint pos_bind = glGetAttribLocation(shaderProgramID, s_inPos.c_str());

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)id);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	if (pos_bind >= 0)
	{
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, 3, GL_FLOAT, GL_FALSE, sizeOfVert, (GLvoid*)offsetof(Vertex_PCT, m_pos));
		ShaderProgramBindProperty(shaderProgramID, s_inColor, 4, GL_TRUE, sizeOfVert, offsetof(Vertex_PCT, m_color));

		//bind point to shader, number of data elements passed, type of data, normalize value passed in, stride (how far between each element), offset of member variable
	}
	GLint pos_UVbind = glGetAttribLocation(shaderProgramID, s_inUV0.c_str());
	if (pos_UVbind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inUV0, 2, GL_FALSE, sizeOfVert, offsetof(Vertex_PCT, m_texCoords));
	}
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	if (iboID >= 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	}

	glBindVertexArray(NULL);
	glUseProgram(NULL);
}
void Renderer::BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID,
	const unsigned int& shaderProgramID, const Vertex_PCTT&)
{
	//Vertex_PCTT version
	const unsigned int sizeOfVert = sizeof(Vertex_PCTT);

	glBindVertexArray(vaoID);

	GLint pos_bind = glGetAttribLocation(shaderProgramID, s_inPos.c_str());

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)id);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	if (pos_bind >= 0)
	{
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, 3, GL_FLOAT, GL_FALSE, sizeOfVert, (GLvoid*)offsetof(Vertex_PCTT, m_pos));
		ShaderProgramBindProperty(shaderProgramID, s_inColor, 4, GL_TRUE, sizeOfVert, offsetof(Vertex_PCTT, m_color));

		//bind point to shader, number of data elements passed, type of data, normalize value passed in, stride (how far between each element), offset of member variable
	}
	//UV
	pos_bind = glGetAttribLocation(shaderProgramID, s_inUV0.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inUV0, 2, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT, m_texCoords));
	}
	//Tangent
	pos_bind = glGetAttribLocation(shaderProgramID, s_inTangent.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inTangent, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT, m_tangent));
	}
	//Bitangent
	pos_bind = glGetAttribLocation(shaderProgramID, s_inBitangent.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inBitangent, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT, m_biTangent));
	}
	//Normal
	pos_bind = glGetAttribLocation(shaderProgramID, s_inNormal.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inNormal, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT, m_normal));
	}
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	if (iboID >= 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	}

	glBindVertexArray(NULL);
	glUseProgram(NULL);
}
void Renderer::BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, 
	const unsigned int& shaderProgramID, const Vertex_PCTT_Skeleton&)
{
	//Vertex_PCTT version
	const unsigned int sizeOfVert = sizeof(Vertex_PCTT_Skeleton);
	glBindVertexArray(vaoID);

	GLint pos_bind = glGetAttribLocation(shaderProgramID, s_inPos.c_str());

	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	//Position and Color
	if (pos_bind >= 0)
	{
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, 3, GL_FLOAT, GL_FALSE, sizeOfVert, (GLvoid*)offsetof(Vertex_PCTT_Skeleton, m_pos));
		ShaderProgramBindProperty(shaderProgramID, s_inColor, 4, GL_TRUE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_color));
		
		//bind point to shader, number of data elements passed, type of data, normalize value passed in, stride (how far between each element), offset of member variable
	}
	pos_bind = glGetAttribLocation(shaderProgramID, s_inBoneIndices.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inBoneWeights, 4, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_boneWeightsAndIndicies.bone_weights));
		glVertexAttribIPointer(pos_bind, 4, GL_INT, sizeOfVert, (GLvoid*)offsetof(Vertex_PCTT_Skeleton, m_boneWeightsAndIndicies.bone_indices));
	}
	//UV
	pos_bind = glGetAttribLocation(shaderProgramID, s_inUV0.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inUV0, 2, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_texCoords));
	}

	//Tangent
	pos_bind = glGetAttribLocation(shaderProgramID, s_inTangent.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inTangent, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_tangent));
	}

	//Bitangent
	pos_bind = glGetAttribLocation(shaderProgramID, s_inBitangent.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inBitangent, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_biTangent));
	}
	
	//Normal
	pos_bind = glGetAttribLocation(shaderProgramID, s_inNormal.c_str());
	if (pos_bind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inNormal, 3, GL_FALSE, sizeOfVert, offsetof(Vertex_PCTT_Skeleton, m_normal));
	}

	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	if (iboID >= 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	}

	glBindVertexArray(NULL);
	glUseProgram(NULL);
}
void Renderer::BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID,
	const unsigned int& shaderProgramID, const SpriteVert&)
{
	//SpriteVert version
	glBindVertexArray(vaoID);
	const unsigned int sizeOfVert = sizeof(SpriteVert);

	GLint pos_bind = glGetAttribLocation(shaderProgramID, s_inPos.c_str());

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)id);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	if (pos_bind >= 0)
	{
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, 2, GL_FLOAT, GL_FALSE, sizeOfVert, (GLvoid*)offsetof(SpriteVert, m_pos));
		ShaderProgramBindProperty(shaderProgramID, s_inColor, 4, GL_TRUE, sizeOfVert, offsetof(SpriteVert, m_color));

		//bind point to shader, number of data elements passed, type of data, normalize value passed in, stride (how far between each element), offset of member variable
	}
	GLint pos_UVbind = glGetAttribLocation(shaderProgramID, s_inUV0.c_str());
	if (pos_UVbind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inUV0, 2, GL_FALSE, sizeOfVert, offsetof(SpriteVert, m_texCoords));
	}
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	if (iboID >= 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	}

	glBindVertexArray(NULL);
}


void Renderer::BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const Particle2D&)
{
	//Particle2D version
	glBindVertexArray(vaoID);
	const unsigned int sizeOfVert = sizeof(Particle2D);

	GLint pos_bind = glGetAttribLocation(shaderProgramID, s_inPos.c_str());

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)id);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/

	if (pos_bind >= 0)
	{
		glEnableVertexAttribArray(pos_bind);
		glVertexAttribPointer(pos_bind, 2, GL_FLOAT, GL_FALSE, sizeOfVert, (GLvoid*)offsetof(Particle2D, position));
		ShaderProgramBindProperty(shaderProgramID, s_inColor, 4, GL_TRUE, sizeOfVert, offsetof(Particle2D, tint));

		//bind point to shader, number of data elements passed, type of data, normalize value passed in, stride (how far between each element), offset of member variable
	}
	GLint pos_UVbind = glGetAttribLocation(shaderProgramID, s_inUV0.c_str());
	if (pos_UVbind >= 0)
	{
		ShaderProgramBindProperty(shaderProgramID, s_inUV0, 2, GL_FALSE, sizeOfVert, offsetof(Particle2D, texCoords));
	}
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	if (iboID >= 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	}

	glBindVertexArray(NULL);
}


//!------
void Renderer::ActivateVAO(const unsigned int& vaoID) const
{
	glBindVertexArray(vaoID);
}

void Renderer::DrawElements(const PrimitiveType& drawMode, const DataType& dataType, const unsigned int& numOfElements, const unsigned int& indicesStartPoint)
{
	GLenum dMode = GetGLenumForPrimType(drawMode);
	GLenum dType = DeteremineDataType(dataType);
	glDrawElements(dMode, numOfElements, dType, (GLvoid*)indicesStartPoint);
}
void Renderer::DeleteVAO(const unsigned int& vaoID)
{
	glDeleteVertexArrays(1, &vaoID);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//VBOs

const unsigned int Renderer::DeteremineVBOUsage(const VBOusage& usage) const
{
	if (usage == STATIC_DRAW)
	{
		return GL_STATIC_DRAW;
	}
	return 0;
}

void Renderer::GenerateOrUpdateVBO_ID(void *data, size_t count, size_t elem_size, unsigned int& id, VBOusage usage)
{
	GenerateBuffer(id);
	UpdateVBO_IDOnly(data, count, elem_size, id, usage);
}

void Renderer::GenerateBuffer(unsigned int& id)
{
	if (id == 0 && glGenBuffers != nullptr)
	{
		glGenBuffers(1, &(GLuint)id);
	}
}

void Renderer::UpdateVBO_IDOnly(void *data, size_t count, size_t elem_size, unsigned int id, VBOusage usage)
{
	unsigned int glUsage = DeteremineVBOUsage(usage);
	if (id == 0)
	{
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, count * elem_size, data, glUsage);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
void Renderer::DeleteVBO_ID(unsigned int& id)
{
	if (id != 0)
	{
		glDeleteBuffers(1, &(GLuint)id);
		id = 0;
	}
}

void Renderer::DrawVBO_PCT(const unsigned int& vboID, const int& numVerts, const PrimitiveType& primType, const bool& hasTexture) const
{
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)vboID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	if (hasTexture == true)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_PCT), (const GLvoid*) offsetof(Vertex_PCT, m_pos));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex_PCT), (const GLvoid*)offsetof(Vertex_PCT, m_color));
	if (hasTexture == true)
	{
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_PCT), (const GLvoid*)offsetof(Vertex_PCT, m_texCoords));
	}

	GLenum type = GetGLenumForPrimType(primType);
	glDrawArrays(type, 0, (GLuint)numVerts);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}
unsigned int GetTextureType(const TextureType& type)
{

	if (type == TEXTURE_2D_TYPE)
	{
		return (unsigned int)GL_TEXTURE_2D;
	}
	else if (type == CUBE_MAP_TYPE)
	{
		return (unsigned int)GL_TEXTURE_CUBE_MAP;
	}
	else if (type == TEXTURE_3D_TYPE)
	{
		return (unsigned int)GL_TEXTURE_3D;
	}
	return (unsigned int)INVALID_TEXTURE;
}
void Renderer::ActivateTexture(const unsigned int& bindPos, const unsigned int& texID, const unsigned int& samplerID, const TextureType& type) const
{
	unsigned int texType = GetTextureType(type);
	glActiveTexture(GL_TEXTURE0 + bindPos);
	glBindTexture(texType, texID);
	glBindSampler(bindPos, samplerID);
}
void Renderer::UnbindTexture(const unsigned int& bindPos, const TextureType& type) const
{
	unsigned int texType = GetTextureType(type);
	glActiveTexture(GL_TEXTURE0 + bindPos);
	glBindTexture(texType, NULL);
}
void Renderer::BindTexture(Texture* toBind) const
{
	if (toBind != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, toBind->m_openglTextureID);
	}
}
void Renderer::BindTexture(const unsigned int& textureID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Renderer::DisableTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindSampler(0, 0);
	glDisable(GL_TEXTURE_2D);
}

//3D related

void Renderer::DrawPoint(const Vector3& position, const float& dotSize, const Rgba& color) const
{
	std::vector<Vertex_PCT> point;
	point.push_back(Vertex_PCT(position, color));
	setPointSize(dotSize);
	DrawVertexArray(point, PRIMITIVE_POINTS, nullptr);
}

void Renderer::DrawLine(const Vector3& position, const Vector3& endPosition, const float& lineSize, const Rgba& color) const
{
	std::vector<Vertex_PCT> line;
	line.push_back(Vertex_PCT(position, color));
	line.push_back(Vertex_PCT(endPosition, color));
	setLineSize(lineSize);
	DrawVertexArray(line, PRIMITIVE_LINES, nullptr);
}

void Renderer::DrawSquaresIn3D(const std::vector<Vector3>& squareBounds, const std::vector<AABB2>& indexBounds, const Texture* texture, const Rgba tint) const
{
	//draws a textured AABB, aka square,
	if (squareBounds.size() != 0 && squareBounds.size() % 4 == 0)
	{
		std::vector<Vertex_PCT> quads;
		{
			int index = 0;
			for (size_t i = 0; i < (size_t)(squareBounds.size() / 4); i++)
			{
				quads.push_back(Vertex_PCT(squareBounds[index], tint));
				quads.push_back(Vertex_PCT(squareBounds[index + 1], tint));
				quads.push_back(Vertex_PCT(squareBounds[index + 2], tint));
				quads.push_back(Vertex_PCT(squareBounds[index + 3], tint));
				if (texture != nullptr)
				{
					AABB2 texCoord = indexBounds[i];
					quads[index].m_texCoords = Vector2(texCoord.mins.x, texCoord.maxs.y);
					quads[index + 1].m_texCoords = Vector2(texCoord.maxs.x, texCoord.maxs.y);
					quads[index + 2].m_texCoords = Vector2(texCoord.maxs.x, texCoord.mins.y);
					quads[index + 3].m_texCoords = Vector2(texCoord.mins.x, texCoord.mins.y);
				}
				index += 4;
			}
		}
		DrawVertexArray(quads, PRIMITIVE_QUADS, texture);
	}
}
void Renderer::DrawVertexArray(const std::vector<Vertex_PCT>& verts, const PrimitiveType& mode, const Texture* spriteSheetTexture) const
{
	int numPointsExpected = GetAmountExpectedForPrimType(mode);
	GLenum type = GetGLenumForPrimType(mode);
	glUseProgram(NULL);
	DisableTexture();
	if (verts.size() > 0 && numPointsExpected != 0 && verts.size() % numPointsExpected == 0)
	{
		bool hasTexture = false;
		if (spriteSheetTexture != nullptr)
		{
			hasTexture = true;
		}
		if (hasTexture == true)
		{
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, spriteSheetTexture->m_openglTextureID);
			glBindSampler(0, 0);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		if (hasTexture == true)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_PCT), &verts[0].m_pos);
		glColorPointer(4, GL_FLOAT, sizeof(Vertex_PCT), &verts[0].m_color);
		if (hasTexture == true)
		{
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_PCT), &verts[0].m_texCoords);
		}

		glDrawArrays(type, 0, verts.size()); //Dies here.

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		DisableTexture();
	}
}
//VBO
//glPushMatrix();
//glTranslatef(m_worldMins.x, m_worldMins.y, m_worldMins.z);
//glBindBuffer(GL_ARRAY_, m_vboiID);
//glEnableClientState(GL_VERTEX_ARRAY);
//glEnableClientState(GL_COLOR_ARRAY);
//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//glVertexPointer(3,GL_FLOAT, sizeof(Vertex3D_PCT), (const GLvoid*) offsetof(Vertex3D_PCT), m_position);
//glColorPointer(4, GL_FLOAT, sizeof(vertex3D_PCT), (const GLvoid*) offsetof(Vertex3D_PCT), m_color);
//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D_PCT), (const GLvoid*) offsetof(Vertex3D_PCT), m_texCoords);
//glDrawArrays(GL_QUADS, 0, vertexes.size());
//glDisableClientState(GL_VERTEX_ARRAY);
//glDisableClientState(GL_COLOR_ARRAY);
//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//DrawAABB2(... texture =nullptr)
//&(((Type*)0)Member)

void Renderer::setLineSize(float size) const
{
	glLineWidth(size);
}
void Renderer::setPointSize(float size) const
{
	glPointSize(size);
}
int Renderer::GetAmountExpectedForPrimType(const PrimitiveType& drawMode) const
{
	int amountOfPointsExpected = 0;
	if (drawMode == PRIMITIVE_POINTS)
	{
		amountOfPointsExpected = 1;
	}
	else if (drawMode == PRIMITIVE_LINES)
	{
		amountOfPointsExpected = 2;
	}
	else if (drawMode == PRIMITIVE_TRIANGLES)
	{
		amountOfPointsExpected = 3;
	}
	else if (drawMode == PRIMITIVE_QUADS)
	{
		amountOfPointsExpected = 4;
	}
	return amountOfPointsExpected;
}
void Renderer::DrawLinedRectangularPrism(const AABB3& bounds, const float& lineThickness, const Rgba& color, const bool& drawCrossedLines) const
{
	std::vector<Vertex_PCT> RectPrism;
	{
		//Bottom
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));


		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));


		//Top
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		//Sides
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

		if (drawCrossedLines == true)
		{
			//Crossed Lines
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.mins.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.mins.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.maxs.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));

			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
			RectPrism.push_back(Vertex_PCT(Vector3(bounds.mins.x, bounds.maxs.y, bounds.maxs.z), Rgba(color.Red, color.Green, color.Blue, color.Alpha)));
		}
		setLineSize(lineThickness);
		DrawVertexArray(RectPrism, PRIMITIVE_LINES, nullptr);
	}
	
}
void Renderer::DepthTestingTrueFalse(const bool& setActive)
{
	if (setActive == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}
void Renderer::CullFaceTrueFalse(const bool& setActive)
{
	if (setActive == true)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void Renderer::turnCameraSoZUpXForward()
{
	//put +Z-up
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	//put +X forward.
	glRotatef(90.f, 0.f, 0.f, 1.f);
}
void Renderer::DrawSimpleCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& tint)
{
	DrawMultiColoredCube(bottomSouthCorner, size, tint, tint, tint, tint, tint, tint);
}

void Renderer::DrawSimpleCubeViaCenter(const Vector3& Center, const EulerAngles& orientation, const Vector3& size, const Rgba& tint, const bool& drawLinedVersion)
{
	Push();
	Translate(Center);
	Rotate(-orientation.m_xRoll, EulerAngles(1.f, 0.f, 0.f));
	Rotate(-orientation.m_yPitch, EulerAngles(0.f, 1.f, 0.f));
	Rotate(-orientation.m_zYaw, EulerAngles(0.f, 0.f, 1.f));
	Vector3 halfSize = size / 2;
	Vector3 bottomLeft = Vector3(0.f,0.f,0.f) - halfSize;
	DrawMultiColoredCube(bottomLeft, size, tint, tint, tint, tint, tint, tint);
	if (drawLinedVersion == true)
	{
		DrawLinedSimpleCube(bottomLeft, size, Rgba(1.f,0.f,1.f,1.f));
	}

	Pop();
}

void Renderer::DrawLinedSimpleCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& tint)
{
	float xAndLength = bottomSouthCorner.x + size.x;
	float yAndWidth = bottomSouthCorner.y + size.y;
	float zAndHeight = bottomSouthCorner.z + size.z;
	std::vector<Vertex_PCT> localLines;
	{
		//bottom
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint));

		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, bottomSouthCorner.z), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint));

		//side
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, bottomSouthCorner.z), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), tint));

		//top
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint));
		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, zAndHeight), tint));

		localLines.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, zAndHeight), tint));
		localLines.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint));
	}
	setLineSize(1.0f);
	DrawVertexArray(localLines, PRIMITIVE_LINES, nullptr);
}
void Renderer::DrawMultiColoredCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& bottomColor, const Rgba& topColor, const Rgba& frontColor,
	const Rgba& backColor, const Rgba& rightSide, const Rgba& leftSide)
{
	//bottom, top, front, back, right, left
	//assume that Camera facing East direction = front.

	float xAndLength = bottomSouthCorner.x + size.x;
	float yAndWidth = bottomSouthCorner.y + size.y;
	float zAndHeight = bottomSouthCorner.z + size.z;
	std::vector<Vertex_PCT> quads;

	{
		//bottom
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), Rgba(bottomColor)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), Rgba(bottomColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), Rgba(bottomColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, bottomSouthCorner.z), Rgba(bottomColor)));

		//top
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), Rgba(topColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), Rgba(topColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), Rgba(topColor)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), Rgba(topColor)));

		//West
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), Rgba(frontColor)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), Rgba(frontColor)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), Rgba(frontColor)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), Rgba(frontColor)));

		//East
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), Rgba(backColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), Rgba(backColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), Rgba(backColor)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, bottomSouthCorner.z), Rgba(backColor)));

		//South
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, bottomSouthCorner.z), Rgba(rightSide)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), Rgba(rightSide)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), Rgba(rightSide)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), Rgba(rightSide)));

		//North
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), Rgba(leftSide)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), Rgba(leftSide)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), Rgba(leftSide)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), Rgba(leftSide)));
	}
	DrawVertexArray(quads, PRIMITIVE_QUADS, nullptr);
}


void Renderer::DrawGridAlongXYPlane(const Vector2& size, const Rgba& color)
{
	Vector3 planeBottomLeftCorner = Vector3(-(size.x/2.f), -(size.y/2.f), 0.f);
	std::vector<Vertex_PCT> localLines;

	{
		//Y lines.
		for (int i = 0; i <= size.y; i++)
		{
			localLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + i, planeBottomLeftCorner.y, planeBottomLeftCorner.z), color));
			localLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + i, planeBottomLeftCorner.y + size.y, planeBottomLeftCorner.z), color));
		}


		//X lines
		for (int i = 0; i <= size.x; i++)
		{
			localLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x, planeBottomLeftCorner.y + i, planeBottomLeftCorner.z), color));
			localLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + size.x, planeBottomLeftCorner.y + i, planeBottomLeftCorner.z), color));
		}
	}
	setLineSize(1.0f);
	DrawVertexArray(localLines, PRIMITIVE_LINES, nullptr);
}

void Renderer::DrawSingleTexturedCube(const Vector3& bottomSouthCorner, const Vector3& size, const Texture* texture, const AABB2 texCoords, const Rgba& tint)
{
	//bottom, top, front, back, right, left
	float xAndLength = bottomSouthCorner.x + size.x;
	float yAndWidth = bottomSouthCorner.y + size.y;
	float zAndHeight = bottomSouthCorner.z + size.z;
	std::vector<Vertex_PCT> quads;

	{
		//bottom
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.z, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));

		//top
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));

		//West side
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));

		//East side
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));

		//South side
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, bottomSouthCorner.y, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));

		//North side
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
		quads.push_back(Vertex_PCT(Vector3(bottomSouthCorner.x, yAndWidth, zAndHeight), tint, Vector2(texCoords.maxs.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, zAndHeight), tint, Vector2(texCoords.mins.x, texCoords.mins.y)));
		quads.push_back(Vertex_PCT(Vector3(xAndLength, yAndWidth, bottomSouthCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y)));
	}
	DrawVertexArray(quads, PRIMITIVE_QUADS, texture);
}
void Renderer::DrawGridLinesNEU(const float& length, const float& width, const float& height, const float& lineThickness)
{
	glDisable(GL_TEXTURE_2D);

	if(lines.size() < 6)
	{
		lines.clear();
		lines.push_back(Vertex_PCT(Vector3(0.f, 0.f, 0.f), Rgba(1.f, 0.f, 0.f, 1.f)));
		lines.push_back(Vertex_PCT(Vector3(length, 0.f, 0.f), Rgba(1.f, 0.f, 0.f, 1.f)));

		lines.push_back(Vertex_PCT(Vector3(0.f,0.f,0.f), Rgba(0.f, 1.f, 0.f, 1.f)));
		lines.push_back(Vertex_PCT(Vector3(0.f, width, 0.f), Rgba(0.f, 1.f, 0.f, 1.f)));

		lines.push_back(Vertex_PCT(Vector3(0.f, 0.f, 0.f), Rgba(0.f, 0.f, 1.f, 1.f)));
		lines.push_back(Vertex_PCT(Vector3(0.f, 0.f, height), Rgba(0.f, 0.f, 1.f, 1.f)));
	}
	else
	{
		lines[1].m_pos.x = length;
		lines[3].m_pos.y = width;
		lines[5].m_pos.z = height;
	}
	setLineSize(lineThickness);
	DrawVertexArray(lines, PRIMITIVE_LINES, nullptr);
}

void Renderer::CreateOpenGLWindow(HINSTANCE& applicationInstanceHandle, const char*& APP_NAME, const int& OFFSET_FROM_WINDOWS_DESKTOP, const int& WINDOW_PHYSICAL_WIDTH,
	const int& WINDOW_PHYSICAL_HEIGHT, HWND& g_hWnd, HDC& g_displayDeviceContext, HGLRC g_openGLRenderingContext, const double& VIEW_LEFT, const double& VIEW_RIGHT,
	const double& VIEW_TOP, const double& VIEW_BOTTOM, WNDCLASSEX& windowClassDescription)
{
	/*
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);
	*/
	//method handles all the disgustingly bad parts of making the app window, just pass it the variables and it you won't have to see how ugly this code really is.

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH,
		OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat(g_displayDeviceContext, &pixelFormatDescriptor);
	SetPixelFormat(g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
	g_openGLRenderingContext = wglCreateContext(g_displayDeviceContext);
	wglMakeCurrent(g_displayDeviceContext, g_openGLRenderingContext);

	//Ortho = orthographic; aka, not 3D. used for 2D games to set up coordinate system.
	//glOrtho(0.f, 1000.f, 10.f, 0.f,0.f,1.f);
	glOrtho(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, 0.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.f);
	glEnable(GL_LINE_SMOOTH);
}
void Renderer::SetOrtho(const Vector2& bottomLeft, const Vector2& topRight, const float& _near, const float& _far)
{
	//sets the general screen location.
	glLoadIdentity();
	glOrtho(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, _near, _far);
}

void Renderer::SetPerspective(const float& fovDegreesY, const float& aspect, const float& nearDist, const float& farDist)
{
	glLoadIdentity();
	gluPerspective(fovDegreesY, aspect, nearDist, farDist);
}

void Renderer::Translate(const Vector2& translateBy)
{
	//translates the screen position
	glTranslatef(translateBy.x, translateBy.y, 0.f);
}
void Renderer::Translate(const Vector3& translateBy)
{
	//translates the screen position
	glTranslatef(translateBy.x, translateBy.y, translateBy.z);
}

void Renderer::Rotate(const float& degrees)
{
	//rotates the screen by degrees, along Z axis only.
	glRotatef(degrees, 0.f, 0.f, 1.f);
}

void Renderer::Rotate(const float& degrees, const EulerAngles& axis)
{
	glRotatef(degrees, axis.m_xRoll, axis.m_yPitch, axis.m_zYaw);
}

void Renderer::Push()
{
	//Pushes in the present screen orientation
	glPushMatrix();
}
void Renderer::Pop()
{
	//pops out the present screen orientation
	glPopMatrix();
}

void Renderer::screenShake(const float& shakeMagnitude)
{
	//suppose to be used to shake the screen.
	float trueDisplacementX = 10.f + (rand() * shakeMagnitude);
	float trueDisplacementY = 10.f + (rand() * shakeMagnitude);
	glTranslatef(trueDisplacementX, trueDisplacementY,0.f);
}
void Renderer::updateCameraPosition(const double& VIEW_LEFT, const double& VIEW_RIGHT, const double& VIEW_BOTTOM, const double& VIEW_TOP)
{
	//updates the camera's position
	glOrtho(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, 0.f, 1.f);
}

void Renderer::clearScreen(const Rgba& color, const bool& clearDepth)
{
	//generates a background screen color so as to prevent a hall of mirrors effect.
	glClearColor(color.Red, color.Green, color.Blue, 1.f);
	if (clearDepth == true)
	{
		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
void Renderer::enableAlphaBlend()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}
void Renderer::disableAlphaBlend()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}