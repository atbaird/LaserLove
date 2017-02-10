#pragma once

#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/RendererEnums.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>


class AABB2;
class AABB3;
struct Vertex_PCT;
struct Vertex_PCTT;
struct Vertex_PCTT_Skeleton;

//-----------------------------------------------------
class Renderer;

//-----------------------------------------------------
extern Renderer* g_Renderer;

class Renderer
{
private:
	std::vector<Vertex_PCT> lines;
public:

	//Common Shader Binds
	static const std::string s_inColor;
	static const std::string s_inPos;
	static const std::string s_inBoneIndices;
	static const std::string s_inBoneWeights;
	static const std::string s_inUV0;
	static const std::string s_inTangent;
	static const std::string s_inBitangent;
	static const std::string s_inNormal;
	static const std::string s_postProcessDiffuseBindName;
	static const std::string s_postProcessDepthBindName;

	//constructor
	static const AABB2 s_WholeTextureCoords;
	IntVector2 ScreenStartAndEnd[2];
	FrameBuffer* m_fbo;

	//Constructors
	Renderer();
	~Renderer();

	//Render shape
	void DrawQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Rgba& color);
	void DrawAABB(const AABB2& bounds, const Rgba& color);
	void DrawQuad3D(const Vector3& position0, Vector3& position1, const Vector3& position2, const Vector3& position3, const Rgba& color);
	void DrawTexturedQuad3D(const Vector3& position0, Vector3& position1, const Vector3& position2, const Vector3& position3, Texture* texture, AABB2 texCoords, const Rgba& tint);
	void DrawTexturedAABB(const AABB2& bounds, const Texture* texture, const AABB2& texCoords, const Rgba& tint);
	void DrawLinedBox(const AABB2& bounds, const Rgba& tint, const float& lineThickness);
	void RenderPolygon(const Vector2& center, float radius, int numSides, float degreesOffSet, Rgba color, bool isDotted = false);
	void RenderLine(const Vector2& startPoint, const Vector2& endPoint, Rgba color, float lineThickness);


	//Screen
	void SetScreenStart(const IntVector2& start);
	void SetScreenEnd(const IntVector2& end);
	const int GetScreenWidth() const;
	const int GetScreenHeight() const;
	const IntVector2 GetStartScreen() const;
	const IntVector2 GetEndScreen() const;
	void SwapBuffer() const;

	//Framebuffers
	void FramebufferBind(FrameBuffer* fbo, const Vector2& scaling = Vector2(1.f,1.f));
	void FramebufferCopyToBack(const FrameBuffer* fbo) const;
	void RenderPostProcess(FrameBuffer* fbo, Material* myMaterial, const Mesh* myMesh) const;

	const unsigned int DeteremineDataType(const DataType& dataType) const;
	const unsigned int DetermineDistanceType(const DistanceType& distanceType) const;

	//Text related
	void DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint = Rgba::s_White, const BitmapFont* font = nullptr, const float& cellAspect = 1.f);

	//Sampler related
	void BindSampler(const unsigned int& samplerID, const unsigned int& units = 0) const;

	//Shader Specific
	void BindTextureToProgram(const unsigned int& samplerID, const unsigned int& bindLoc, const unsigned int& textureID) const;
	const bool RunShaderProgram(const unsigned int& vaoID, const unsigned int& shaderProgID, const unsigned int& numberOfElemToDraw, const PrimitiveType& primitiveType, const DataType& IBODataType) const;
	const unsigned int DetermineIfFragOrVertShader(const std::string& fileLocation) const;
	const unsigned int CreateShader(const unsigned int& shaderType);
	const unsigned int CompileShader(const std::string& fileLocation, bool& compiledShaderSuccessfully);
	void DeleteShader(const unsigned int& shaderID);
	const unsigned int CreateAndLinkShaderProgram(const unsigned int& vertexShaderID, const unsigned int& fragmentShaderID);
	void ShaderProgramBindProperty(const unsigned int& program, const std::string& name, const int& count, const bool& normalize, const int& stride, const int& offset, const DataType& type = FLOATING_POINT_);
	void DeleteShaderProgram(const unsigned int& shaderProgramID);
	void SetUseShaderProgram(const unsigned int& programId);
	void DeactivateShaderProgram();
	const int GetUniformLocationFromShaderProgramByName(const unsigned int& programID, const std::string& variableName) const;
	void SetVariableLocToValueFourFloatPointer(const int& Value, const Vector4* toSet, const int& numberOfValues = 1) const;
	void SetVariableLocToValueThreeFloatPointer(const int& Value, const Vector3* toSet, const int& numberOfValues = 1) const;
	void SetVariableLocToValueTwoFloatPointer(const int& Value, const Vector2* toSet, const int& numberOfValues = 1) const;
	void SetVariableLocToValueOneFloatPointer(const int& Value, const float* toSet, const int& numberOfValues = 1) const;
	void SetVariableLocToValueOneIntPointer(const int& Value, const int* toSet, const int& numberOfValues = 1) const;
	void SetVariablesLocToValue4By4Matrix(const int& Value, const Matrix44* toSet, const bool& transpose = false, const int& numberOfValues = 1) const;
	const unsigned int CreateSampler(const DistanceType& min_tex_filter, const DistanceType& mag_tex_filter, const DistanceType& u_wrap, const DistanceType& v_wrap);
	
	//VAOs
	void GenerateVAO(unsigned int& vaoIDGenerated);

	//!-----
	//BindMeshToVAOs
	void BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const Vertex_PCT&);
	void BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const Vertex_PCTT&);
	void BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const Vertex_PCTT_Skeleton&);
	void BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const SpriteVert&);
	void BindMeshToVAO(const unsigned int& vaoID, const unsigned int& vboID, const unsigned int& iboID, const unsigned int& shaderProgramID, const Particle2D&);

	//!-----
	void ActivateVAO(const unsigned int& vaoID = NULL) const;
	void DrawElements(const PrimitiveType& drawMode, const DataType& dataType, const unsigned int& numOfElements, const unsigned int& indicesStartPoint);
	void DeleteVAO(const unsigned int& vaoID);

	//VBOs
	const unsigned int DeteremineVBOUsage(const VBOusage& usage) const;
	void GenerateOrUpdateVBO_ID(void *data, size_t count, size_t elem_size, unsigned int& id, VBOusage usage = STATIC_DRAW);
	void GenerateBuffer(unsigned int& id);
	void UpdateVBO_IDOnly(void *data, size_t count, size_t elem_size, unsigned int id, VBOusage usage = STATIC_DRAW);
	void DeleteVBO_ID(unsigned int& id);
	void DrawVBO_PCT(const unsigned int& vboID, const int& numVerts, const PrimitiveType& primType, const bool& hasTexture = true) const;
	void ActivateTexture(const unsigned int& bindPos, const unsigned int& texID, const unsigned int& samplerID, const TextureType& type = TEXTURE_2D_TYPE) const;
	void UnbindTexture(const unsigned int& bindPos, const TextureType& type = TEXTURE_2D_TYPE) const;
	void BindTexture(const unsigned int& textureID);
	void BindTexture(Texture* toBind) const;
	void DisableTexture() const;

	//3D related
	void DrawPoint(const Vector3& position, const float& dotSize = 1.f, const Rgba& color = Rgba::s_White) const;
	void DrawLine(const Vector3& position, const Vector3& endPosition, const float& lineSize = 1.f, const Rgba& color = Rgba::s_White) const;
	void DrawSquaresIn3D(const std::vector<Vector3>& squareBounds, const std::vector<AABB2>& indexBounds, const Texture* texture = nullptr, const Rgba tint = Rgba::s_White) const;
	void DrawLinedRectangularPrism(const AABB3& bounds, const float& lineThickness = 1.f, const Rgba& color = Rgba::s_White, const bool& drawCrossedLines = false) const;
	void DrawVertexArray(const std::vector<Vertex_PCT>& verts, const PrimitiveType& mode, const Texture* spriteSheetTexture = nullptr) const;
	void setLineSize(float size) const;
	void setPointSize(float size) const;
 	int GetAmountExpectedForPrimType(const PrimitiveType& drawMode) const;
// 	void renderABunchOfQuadsViaVertexArrays(const std::vector<Vertex_PCT>& quadVertices, const Texture* spriteSheetTexture = nullptr, bool hasTexture = false) const;
// 	void renderABunchOfPointsViaVertexArrays(const std::vector<Vertex_PCT>& pointVertices, const float& dotSize = 1.0f) const;
// 	void renderABunchOfLinesViaVertexArrays(const std::vector<Vertex_PCT>& lineVertices, const float& lineWidth = 1.0f, const bool& isDotted = false) const;
	void DepthTestingTrueFalse(const bool& setActive = true);
	void CullFaceTrueFalse(const bool& setActive = true);
	void turnCameraSoZUpXForward();
	void DrawSimpleCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& tint);
	void DrawSimpleCubeViaCenter(const Vector3& Center, const EulerAngles& orientation, const Vector3& size, const Rgba& tint, const bool& drawLines);
	void DrawLinedSimpleCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& tint);
	void DrawMultiColoredCube(const Vector3& bottomSouthCorner, const Vector3& size, const Rgba& bottomColor, const Rgba& topColor, const Rgba& frontColor,
		const Rgba& backColor, const Rgba& rightSide, const Rgba& leftSide);
	void DrawGridAlongXYPlane(const Vector2& size, const Rgba& color);
	void DrawSingleTexturedCube(const Vector3& bottomSouthCorner, const Vector3& size, const Texture* texture, const AABB2 texCoords, const Rgba& tint);
	void DrawGridLinesNEU(const float& length, const float& width, const float& height, const float& lineThickness = 1.f);

	//Window and screen
	void CreateOpenGLWindow(HINSTANCE& applicationInstanceHandle, const char*& APP_NAME, const int& OFFSET_FROM_WINDOWS_DESKTOP, const int& WINDOW_PHYSICAL_WIDTH,
		const int& WINDOW_PHYSICAL_HEIGHT, HWND& g_hWnd, HDC& g_displayDeviceContext, HGLRC g_openGLRenderingContext, const double& VIEW_LEFT, const double& VIEW_RIGHT, 
		const double& VIEW_TOP, const double& VIEW_BOTTOM, WNDCLASSEX& windowClassDescription);
	void SetOrtho(const Vector2& bottomLeft, const Vector2& topRight, const float& _near = 0.f, const float& _far = 1.f);
	void SetPerspective(const float& fovDegreesY, const float& aspect, const float& nearDist, const float& farDist);
	void clearScreen(const Rgba& color, const bool& clearDepth = false);
	void Translate(const Vector2& translateBy);
	void Translate(const Vector3& translateBy);
	void Rotate(const float& degrees);
	void Rotate(const float& degrees, const EulerAngles& axis);
	void Push();
	void Pop();
	void screenShake(const float& shakeMagnitude);
	void updateCameraPosition(const double& VIEW_LEFT, const double& VIEW_RIGHT, const double& VIEW_BOTTOM, const double& VIEW_TOP);
	void enableAlphaBlend();
	void disableAlphaBlend();

private:
	void loadExternalMethods();
};