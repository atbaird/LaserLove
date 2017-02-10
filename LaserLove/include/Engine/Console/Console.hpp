#pragma once
#include "Engine/Math/Vertex_PCT.hpp"
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
public:
	Console(Font* fontToUse = nullptr, const bool& responsibleForDeletingFont = false);
	~Console();

	void Update(float deltaSeconds);
	void Render() const;

	void ChangeFont(Font* fontToUse);
	void SetResponsibleForDeletingFont(const bool& responsible) { m_responsible = responsible; };
	void ConsolePrint(const std::string& input, const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const bool& backgroundBox = false, const Rgba& backgroundColor = Rgba(0.f,0.f,0.f,1.f));
	void ConsolePrintf(const ConsoleImportanceLevel& importanceLevel, const std::string& consoleMessage);
	static const bool RegisterCommand(const std::string& command_name, console_command_cb callback, const console_command_cb& helpMessage);
	const bool RunCommand(const std::string& command_line);

	void setConsoleActive(const bool& active) { consoleActive = active; };
	const bool isConsoleActive() const { return consoleActive; };

	const bool isQuitting() const;

	void clear();
	void quit() { quitApplication = true; };
	void help(const Command& args);

	Font* GetFont() const;


	static std::vector<SpecialLine, TUntrackedAllocator<SpecialLine>> commandPromptLines;
private:

	void UpdateLineText();
	void UpdatePresentlyBeingTypedText();
	void UpdateCursorText();
	const bool guaranteeKeySpecialCase(const unsigned char& input) const;
	void handleKeyPressEventsCorrectly(const unsigned char& input);
	void handleShiftCaseChanges(size_t& endVal);
	void testAllKeysForPushedDown();
	void UpdateCursor(const float& deltaSeconds);

	void RenderCommandPromptLines() const;
	void RenderCurrentlyBeingTypedText() const;
	void RenderCursor() const;

	bool consoleActive;
	bool m_responsible;
	bool cursorOn;
	int cursorPosition;
	float m_fontSize;
	float m_secondsSinceLastToggledCursor;
	int currentFirstViewable;
	size_t maxNumLines;
	Vector2 assumedAspects;
	Vector2 commandPromptBoxStart;
	Vector2 commandPromptBoxEnd;
	Vector2 commandLineStart;
	Vector2 inputBoxStart;
	Vector2 inputBoxLineStart;
	Rgba currentPushColor;
	Font* m_consoleFont;
	std::vector<Vertex_PCT> overlayVertices;
	std::vector<std::vector<Vertex_PCT>> lineVertices;
	std::vector<std::vector<Vertex_PCT>> beingTypedVertices;
	std::vector<std::vector<Vertex_PCT>> cursorVertices;
	std::vector<bool> m_whatKeysHeldDown;
	std::string currentlyBeingTyped;

	bool quitApplication;

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
	//static std::map<std::string, console_command_cb> gCommands;
};
#endif // !CONSOLE_HPP
