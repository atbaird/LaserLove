#pragma once
#include "Engine/Math/Vertices/Vertex_PCT.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/AConsoleBuildMeFirsts.hpp"
#include <vector>
#include <map>

#ifndef aCONSOLEGLOBALS_HPP
#define aCONSOLEGLOBALS_HPP


#endif


#ifndef CONSOLE_HPP
#define CONSOLE_HPP
//-----------------------------------------------------
class Console;

//-----------------------------------------------------
extern Console* g_Console;


struct SpecialLine
{
	SpecialLine(const std::string& text = "", const Rgba& color = Rgba(1.f,1.f,1.f,1.f), const bool& drawTextInBox = false, const Rgba& boxRGBA = Rgba(0.f,0.f,0.f,1.f));
	Rgba lineColor;
	std::string line;
	bool drawInBox;
	Rgba boxColor;
};



enum ConsoleImportanceLevel
{
	INVALID_IMPORTANCE = -1,
	UNIMPORTANT,
	WARNING,
	URGENT
};
class Console
{
private:
	//Static Variables
	static const float s_secondsBetweenToggleCursorView;
	static const Rgba s_defaultColor;
	static const std::string s_brokenBar;
	static const Vector3 s_rightDirection;
	static const Vector3 s_upDirection;
	static const Rgba s_noImportanceLevel_boxcolor;
	static const Rgba s_UNIMPORTANT_boxcolor;
	static const Rgba s_URGENT_boxcolor;
	static const Rgba s_UNIMPORTANT_textcolor;
	static const Rgba s_WARNING_textcolor;
	static const Rgba s_URGENT_textcolor;

	//Variables
	bool m_quitApplication = false;
	bool m_consoleActive = false;
	bool m_responsible = false;
	bool m_ranCommand = false;
	bool m_cursorOn = true;
	size_t m_maxNumLines = 15;
	int cursorPosition = 0;
	int currentFirstViewable = 0;
	Texture* m_defaultTexture = nullptr;
	Font* m_consoleFont = nullptr;
	float m_fontSize = 0.01f;
	float m_secondsSinceLastToggledCursor = 0.f;
	
	//Modelcreation pieces
	Vector2 assumedAspects = Vector2(16.f, 9.f);
	Vector2 commandPromptBoxStart = Vector2(0.f, 0.f);
	Vector2 commandPromptBoxEnd = Vector2(0.f, 0.f);
	Vector2 commandLineStart = Vector2(0.f, 0.f);
	Vector2 inputBoxLineStart = Vector2(0.f, 0.f);
	Vector3 overallTranslation = Vector3(0.f, 0.f, 0.f);
	Vector3 whiteBoxTranslation = Vector3(0.f, 0.f, 0.f);
	Vector3 grayBoxTranslation = Vector3(0.f, 0., 0.f);
	Vector3 inputBoxTranslation = Vector3(0.f, 0.f, 0.f);
	Vector3 startOfInputLineTranslation = Vector3(-8.f, -0.9f);

	Vector3 whiteBoxGeneralTranslation  = Vector3(0.f, 0.f, 0.f);
	Vector3 commandPromptBoxTranslation = Vector3(0.f, 0.f, 0.f);
	Vector3 grayBoxGeneralTranslation = Vector3(0.f, 0.f, 0.f);
	Vector3 inputBoxGeneralTranslation = Vector3(0.f, -3.5f);
	Vector3 commandPromptBoxGeneralTranslation = Vector3(0.f, 0.3f);


	Rgba currentPushColor = Rgba(1.f,1.f,1.f,1.f);
	Rgba whiteBoxColor = Rgba(1.f, 1.f, 1.f, 0.7f);
	Rgba inputBoxColor = Rgba(0.f, 0.f, 0.f, 0.7f);
	Rgba grayBoxColor = Rgba(0.5f, 0.5f, 0.5f, 0.7f);
	Rgba commandPromptBoxColor = Rgba(0.f, 0.f, 0.f, 0.7f);
	std::string currentlyBeingTyped = "";
	Matrix44 m_Projection;
	Matrix44 m_View;
	Matrix44 m_GeneralModel;
	Mesh m_overlaymesh;
	std::vector<Mesh> m_lineMesh;
	std::vector<Mesh> m_beingTypedMesh;
	std::vector<Mesh> m_cursorMesh;
	Material* m_overlayMat = nullptr;
	ShaderProgram* m_fontShader = nullptr;
	std::vector<Material> m_textMat;
	std::vector<bool> m_whatKeysHeldDown;
	std::vector<Vertex_PCT> overlayVertices;
	std::vector<std::vector<unsigned int>> lineIndices;
	std::vector<std::vector<unsigned int>> beingTypedIndices;
	std::vector<std::vector<unsigned int>> cursorIndices;
	std::vector<std::vector<Vertex_PCT>> lineVertices;
	std::vector<std::vector<Vertex_PCT>> beingTypedVertices;
	std::vector<std::vector<Vertex_PCT>> cursorVertices;

protected:
	void RecreateGeneralModel();
public:
	Console(Font* fontToUse = nullptr, const bool& responsibleForDeletingFont = false,
		ShaderProgram* prog = nullptr,
		Texture* whiteBoxImage = nullptr);
	~Console();

	void Update(float deltaSeconds);
	void Render() const;

	void ChangeFont(Font* fontToUse);
	void SetResponsibleForDeletingFont(const bool& responsible) { m_responsible = responsible; };
	void ConsolePrint(const std::string& input, const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const bool& backgroundBox = false, const Rgba& backgroundColor = Rgba(0.f,0.f,0.f,1.f));
	void ConsolePrintf(const ConsoleImportanceLevel& importanceLevel, const std::string& consoleMessage);
	static const bool RegisterCommand(const std::string& command_name, console_command_cb callback, const console_command_cb& helpMessage);
	const bool RunCommand(const std::string& command_line);

	void SetConsoleActive(const bool& active) { m_consoleActive = active; };
	const bool IsConsoleActive() const { return m_consoleActive; };

	const bool IsQuitting() const;
	const size_t GetNumberOfCommandPromptLines() const;
	const bool GetRanCommand() const;
	const std::vector<std::string> GetMessages(size_t start, size_t end);

	void Clear();
	void quit() { m_quitApplication = true; };
	void Help(const Command& args);

	Font* GetFont() const;


	static std::vector<SpecialLine, TUntrackedAllocator<SpecialLine>> commandPromptLines;
private:

	void UpdateLineText();
	void UpdatePresentlyBeingTypedText();
	void UpdateCursorText();
	const bool GuaranteeKeySpecialCase(const unsigned char& input) const;
	void HandleKeyPressEventsCorrectly(const unsigned char& input);
	void HandleShiftCaseChanges(size_t& endVal);
	void TestAllKeysForPushedDown();
	void UpdateCursor(const float& deltaSeconds);

	void RenderCommandPromptLines() const;
	void RenderCurrentlyBeingTypedText() const;
	void RenderCursor() const;
	//static std::map<std::string, console_command_cb> gCommands;
};
#endif // !CONSOLE_HPP
