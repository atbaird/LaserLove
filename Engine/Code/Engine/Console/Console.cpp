
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"




//--------------------------------------------
Console* g_Console = nullptr;
std::vector<SpecialLine, TUntrackedAllocator<SpecialLine>> Console::commandPromptLines;

const float Console::s_secondsBetweenToggleCursorView = 0.5f;
const Rgba Console::s_defaultColor = Rgba(1.f,1.f,1.f,1.f);
const std::string Console::s_brokenBar = "|";
const Vector3 Console::s_rightDirection = Vector3(1.f, 0.f, 0.f);
const Vector3 Console::s_upDirection = Vector3(0.f, 1.f, 0.f);
const Rgba Console::s_noImportanceLevel_boxcolor = Rgba(0.f, 0.f, 0.f, 0.f);
const Rgba Console::s_UNIMPORTANT_boxcolor = Rgba(1.f, 1.f, 1.f, 0.f);
const Rgba Console::s_URGENT_boxcolor = Rgba(1.f, 1.f, 0.f, 1.f);
const Rgba Console::s_UNIMPORTANT_textcolor = Rgba(1.f, 1.f, 1.f, 1.f);
const Rgba Console::s_WARNING_textcolor = Rgba(1.f, 1.f, 0.f, 1.f);
const Rgba Console::s_URGENT_textcolor = Rgba(1.f, 0.f, 0.f, 1.f);

SpecialLine::SpecialLine(const std::string& text, const Rgba& color, const bool& drawTextInBox, const Rgba& boxRGBA)
	: lineColor(color),
	line(text),
	drawInBox(drawTextInBox),
	boxColor(boxRGBA)
{
}

Console::Console(Font* fontToUse, const bool& responsibleForDeletingFont, ShaderProgram* prog,
	Texture* whiteBoxImage)
	: m_consoleFont(fontToUse),
	m_responsible(responsibleForDeletingFont),
	currentPushColor(s_defaultColor)
{
	m_GeneralModel.MatrixMakeIdentity();
	m_fontShader = prog;
	m_defaultTexture = whiteBoxImage;
	m_overlayMat = new Material();
	m_overlayMat->SetProgram(m_fontShader);
	m_overlayMat->SetOrAddTexture(m_defaultTexture, 0, "gDiffuseTex", 0);

	RecreateGeneralModel();

	ConsolePrint("Game, running.", Rgba(0.f, 1.f, 0.f, 1.f), true, Rgba(0.f, 0.f, 1.f, 1.f));
	ConsolePrint("Console text, optimal.", Rgba(0.f, 0.f, 1.f, 1.f));
	ConsolePrint("CONSOLE ONLINE!", Rgba(1.f, 0.f, 0.f, 1.f));


	//RegisterCommand("help", Help);
	//RegisterCommand("quit", Quit);
	//RegisterCommand("clear", Clear);
	static RegisterCommandHelper _helpHelper("help", ::Help, HelpHelpMessage);
	static RegisterCommandHelper _quitHelper("quit", ::Quit, QuitHelpMessage);
	static RegisterCommandHelper _clearHelper("clear", ::Clear, ClearHelpMessage);

	m_whatKeysHeldDown.resize(NUM_KEYS);
	for (size_t index = 0; index < NUM_KEYS; index++)
	{
		m_whatKeysHeldDown[index] = false;
	}
}


void Console::RecreateGeneralModel()
{
	Vector2 inputBoxStart = Vector2(0.75f, 0.65f);
	Vector2 whiteBoxStart = Vector2(0.2f, 0.2f);
	Vector2 grayBoxStart = Vector2(0.5f, 0.5f);

	Vector2 whiteBoxEnd = Vector2(assumedAspects.x - whiteBoxStart.x, assumedAspects.y - whiteBoxStart.y);
	Vector2 grayBoxEnd = Vector2(assumedAspects.x - grayBoxStart.x, assumedAspects.y - grayBoxStart.y);
	Vector2 inputBoxEnd = Vector2(assumedAspects.x - inputBoxStart.x, inputBoxStart.y + 0.5f);

	inputBoxLineStart = Vector2(inputBoxStart.x, inputBoxStart.y + 0.05f);
	commandPromptBoxStart = Vector2(inputBoxStart.x, inputBoxEnd.y + 0.2f);
	commandPromptBoxEnd = Vector2(inputBoxEnd.x, grayBoxEnd.y - 0.2f);
	commandLineStart = Vector2(commandPromptBoxStart.x, commandPromptBoxStart.y);


	whiteBoxTranslation = whiteBoxGeneralTranslation + overallTranslation;
	grayBoxTranslation = grayBoxGeneralTranslation + overallTranslation;
	inputBoxTranslation = inputBoxGeneralTranslation + overallTranslation;
	commandPromptBoxTranslation = commandPromptBoxGeneralTranslation + overallTranslation;

	MeshBuilder combinedBuilder;
	MeshBuilder builder;
	Vector3 up = Vector3(0.f, 1.f);
	Vector3 right = Vector3(1.f);
	builder.BuildQuad(up, right, whiteBoxEnd.x - whiteBoxStart.x, whiteBoxEnd.y - whiteBoxStart.y, 1, 1,
		whiteBoxColor, whiteBoxTranslation);
	combinedBuilder.AddPointsAndIndices(&builder);
	builder.BuildQuad(up, right, grayBoxEnd.x - grayBoxStart.x, grayBoxEnd.y - grayBoxStart.y, 1, 1,
		grayBoxColor, grayBoxTranslation);
	combinedBuilder.AddPointsAndIndices(&builder);
	builder.BuildQuad(up, right, commandPromptBoxEnd.x - commandPromptBoxStart.x, commandPromptBoxEnd.y - commandPromptBoxStart.y, 1, 1,
		commandPromptBoxColor, commandPromptBoxTranslation);
	combinedBuilder.AddPointsAndIndices(&builder);
	builder.BuildQuad(up, right, inputBoxEnd.x - inputBoxStart.x, inputBoxEnd.y - inputBoxStart.y, 1, 1,
		inputBoxColor, inputBoxTranslation);
	combinedBuilder.AddPointsAndIndices(&builder);
	combinedBuilder.CopyOnToMeshVertexPCT(&m_overlaymesh);
}

Console::~Console()
{
	if (m_overlayMat != nullptr)
	{
		delete m_overlayMat;
		m_overlayMat = nullptr;
	}
	if (m_responsible == true && m_consoleFont != nullptr)
	{
		Font::DeleteFont(m_consoleFont);
		m_consoleFont = nullptr;
	}
	overlayVertices.clear();
	commandPromptLines.clear();
	m_whatKeysHeldDown.clear();
	if (ConsoleGlobals::m_commands != nullptr)
	{
		ConsoleGlobals::m_commands->clear();
		delete ConsoleGlobals::m_commands;
		ConsoleGlobals::m_commands = nullptr;
	}
	if (ConsoleGlobals::m_helpMessages != nullptr)
	{
		ConsoleGlobals::m_helpMessages->clear();
		delete ConsoleGlobals::m_helpMessages;
		ConsoleGlobals::m_helpMessages = nullptr;
	}
}

void Console::ChangeFont(Font* fontToUse)
{
	m_consoleFont = nullptr;
	m_consoleFont = fontToUse;
	UpdateLineText();
	UpdatePresentlyBeingTypedText();
	UpdateCursorText();
}

void Console::Update(float deltaSeconds)
{
	m_Projection.MatrixMakeIdentity();
	m_View.MatrixMakeIdentity();

	Matrix44::MakeEulerRotation(m_View, (0.f), (-0.f), (0.f), Vector3::vec3_zeros);
	Matrix44::MatrixInvertOrthogonal(m_View);

	float width = (float)g_Renderer->GetScreenWidth() / 100.f;
	float height = (float)g_Renderer->GetScreenHeight() / 100.f;
	Matrix44::MatrixMakeOrthogonal(m_Projection, Vector2(width, height), -1.f, 1.f);



	g_input->setCursorShow(true);
	UpdateCursor(deltaSeconds);
	TestAllKeysForPushedDown();

	for (size_t i = 0; i < m_whatKeysHeldDown.size(); i++)
	{
		if (g_input->IsKeyDown((unsigned char)i) == false)
		{
			m_whatKeysHeldDown[i] = false;
		}
	}
}

const bool Console::GuaranteeKeySpecialCase(const unsigned char& input) const
{
	if (input == tilde
		|| input == VK_LEFT
		|| input == VK_RIGHT
		|| input == VK_UP
		|| input == VK_DOWN
		|| input == VK_RBUTTON
		|| input == VK_LBUTTON
		|| input == VK_SHIFT
		|| input == VK_RETURN
		|| input == VK_BACK
		|| input == VK_DELETE
		|| input == VK_MBUTTON
		|| input == VK_CONTROL
		|| input == VK_CAPITAL
		|| input == VK_TAB
		|| input == VK_END
		|| input == VK_HOME)
	{
		return true;
	}
	return false;
}

void Console::TestAllKeysForPushedDown()
{
	for (size_t i = 0; i < m_whatKeysHeldDown.size(); i++)
	{
		if (m_whatKeysHeldDown[i] == false && g_input->IsKeyDown((unsigned char)i))
		{
			m_whatKeysHeldDown[i] = true;
			HandleKeyPressEventsCorrectly((unsigned char)i);
		}
	}
}


void Console::HandleKeyPressEventsCorrectly(const unsigned char& input)
{
	if (GuaranteeKeySpecialCase(input) == false)
	{
		std::string firstHalf = currentlyBeingTyped.substr(0, cursorPosition);
		std::string secondHalf = currentlyBeingTyped.substr(cursorPosition, currentlyBeingTyped.length() - cursorPosition);
		size_t endVal = input;
		HandleShiftCaseChanges(endVal);

		unsigned char newChar = ((unsigned char)endVal);
		std::string newCharString = "" + std::string(1, newChar);
		currentlyBeingTyped = firstHalf + newCharString + secondHalf;
		UpdatePresentlyBeingTypedText();
		cursorPosition++;
		UpdateCursorText();
	}
	else if (input == VK_BACK && cursorPosition != 0)
	{
		std::string firstHalf = currentlyBeingTyped.substr(0, cursorPosition - 1);
		std::string secondHalf = currentlyBeingTyped.substr(cursorPosition, currentlyBeingTyped.length() - cursorPosition);
		currentlyBeingTyped = firstHalf + secondHalf;
		UpdatePresentlyBeingTypedText();
		cursorPosition--;
		UpdateCursorText();
	}
	else if (input == VK_DELETE && (size_t)cursorPosition != currentlyBeingTyped.length())
	{
		std::string firstHalf = currentlyBeingTyped.substr(0, cursorPosition);
		std::string secondHalf = currentlyBeingTyped.substr(cursorPosition + 1, currentlyBeingTyped.length() - cursorPosition);
		currentlyBeingTyped = firstHalf + secondHalf;
		UpdatePresentlyBeingTypedText();
	}
	else if (input == VK_RETURN)
	{
		ConsolePrint(currentlyBeingTyped, currentPushColor);
		RunCommand(currentlyBeingTyped);
		currentlyBeingTyped = "";
		cursorPosition = 0;
		UpdateLineText();
		UpdatePresentlyBeingTypedText();
		UpdateCursorText();
	}
	else if (input == VK_LEFT)
	{
		cursorPosition -= 1;

		m_secondsSinceLastToggledCursor = 0.f;
		m_cursorOn = true;
		if (cursorPosition < 0)
		{
			cursorPosition = 0;
		}
		UpdateCursorText();
	}
	else if (input == VK_RIGHT)
	{
		cursorPosition += 1;

		m_secondsSinceLastToggledCursor = 0.f;
		m_cursorOn = true;
		if (cursorPosition > (int)currentlyBeingTyped.length())
		{
			cursorPosition = currentlyBeingTyped.length();
		}
		UpdateCursorText();
	}
	else if (input == VK_END)
	{
		cursorPosition = currentlyBeingTyped.length();
		UpdateCursorText();
	}
	else if (input == VK_HOME)
	{
		cursorPosition = 0;
		UpdateCursorText();
	}
	else if (input == VK_UP)
	{
		currentFirstViewable -= 1;
		if (currentFirstViewable < 0)
		{
			currentFirstViewable = 0;
		}
		UpdateLineText();
	}
	else if (input == VK_DOWN)
	{
		currentFirstViewable += 1;
		if (currentFirstViewable >= (int)commandPromptLines.size())
		{
			currentFirstViewable = commandPromptLines.size() - 1;
		}
		UpdateLineText();
	}
}

void Console::HandleShiftCaseChanges(size_t& endVal)
{
	if (g_input->IsKeyDown(VK_SHIFT))
	{
		if (endVal == 48)
		{
			endVal = 33;
		}

		switch (endVal)
		{
		case '0':
			endVal = ')';
			break;
		case '1':
			endVal = '!';
			break;
		case '2':
			endVal = '@';
			break;
		case '3':
			endVal = '#';
			break;
		case '4':
			endVal = '$';
			break;
		case '5':
			endVal = '%';
			break;
		case '6':
			endVal = '^';
			break;
		case '7':
			endVal = '&';
			break;
		case '8':
			endVal = '*';
			break;
		case '9':
			endVal = '(';
			break;
		case 190:
			endVal = '>';
			break;
		case 188:
			endVal = '<';
			break;
		case 189:
			endVal = '_';
			break;
		case '\'':
			endVal = '\"';
			break;
		case 186:
			endVal = ':';
			break;
		case '[':
			endVal = '{';
			break;
		case ']':
			endVal = '}';
			break;
		case 220:
			endVal = '|';
			break;
		case 191:
			endVal = '?';
			break;
		default:
			break;
		}
	}
	else
	{
		if (endVal >= 65 && endVal <= 90)
		{
			endVal += 32;
		}
		if (endVal == 190)
		{
			endVal = '.';
		}
		if (endVal == 220)
		{
			endVal = '\\';
		}
		if (endVal == 191)
		{
			endVal = '/';
		}
		if (endVal == 188)
		{
			endVal = ',';
		}
		if (endVal == 189)
		{
			endVal = '-';
		}
	}
}

void Console::UpdateCursor(const float& deltaSeconds)
{
	m_secondsSinceLastToggledCursor += deltaSeconds;
	if (m_secondsSinceLastToggledCursor >= s_secondsBetweenToggleCursorView)
	{
		m_secondsSinceLastToggledCursor = 0.f;
		m_cursorOn = !m_cursorOn;
	}
}

void Console::Clear()
{
	commandPromptLines.clear();
	currentFirstViewable = 0;
}

void Console::Help(const Command& args)
{
	if (ConsoleGlobals::m_helpMessages == nullptr)
	{
		return;
	}
	std::string var = args.get_string_by_index(0);
	var = args.make_string_lower_case(var);
	std::map<std::string, console_command_cb>::const_iterator helpIter = ConsoleGlobals::m_helpMessages->find(var);
	if (strcmp(var.c_str(), "") == 0)
	{
		for (std::map<std::string, console_command_cb>::const_iterator commandIter = ConsoleGlobals::m_commands->begin(); commandIter != ConsoleGlobals::m_commands->end(); commandIter++)
		{
			ConsolePrint(commandIter->first);
		}
	}
	else if (helpIter != ConsoleGlobals::m_helpMessages->end() && helpIter->second != nullptr)
	{
		Command arg = args;
		helpIter->second(arg);
	}
}

Font* Console::GetFont() const
{
	return m_consoleFont;
}

void Console::ConsolePrint(const std::string& input, const Rgba& color, const bool& backgroundBox, const Rgba& backgroundColor)
{
	commandPromptLines.push_back(SpecialLine(input, color, backgroundBox, backgroundColor));
	currentFirstViewable = commandPromptLines.size() - 1;
	UpdateLineText();
}

void Console::ConsolePrintf(const ConsoleImportanceLevel& importanceLevel, const std::string& consoleMessage)
{
	Rgba textColor = Rgba::s_White;
	Rgba boxColor = s_noImportanceLevel_boxcolor;
	bool useBoxColor = false;
	if (importanceLevel == UNIMPORTANT)
	{
		textColor = s_UNIMPORTANT_textcolor;
		boxColor = s_UNIMPORTANT_boxcolor;
		useBoxColor = false;
	}
	else if (importanceLevel == WARNING)
	{
		textColor = s_WARNING_textcolor;
	}
	else if (importanceLevel == URGENT)
	{
		textColor = s_URGENT_textcolor;
		boxColor = s_URGENT_boxcolor;
		useBoxColor = true;
	}
	ConsolePrint(consoleMessage, textColor, useBoxColor, boxColor);
}

const bool Console::RegisterCommand(const std::string& command_name, console_command_cb callback, const console_command_cb& helpMessage)
{
	if (SimpleStrCmp(command_name, "") == true)
	{
		const std::string warning = "Will not register a command with no name.";
		DebuggerPrintf(warning.c_str());
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, warning.c_str());
		}
		return false;
	}
	else if (callback == nullptr)
	{
		const std::string warning = "Will not register a command to a null method.";
		DebuggerPrintf(warning.c_str());
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, warning.c_str());
		}
		return false;
	}
	std::string true_command_name = Command::make_string_lower_case(command_name);

	if (ConsoleGlobals::m_commands == nullptr)
	{
		ConsoleGlobals::m_commands = new std::map<std::string, console_command_cb>();
	}
	if (ConsoleGlobals::m_helpMessages == nullptr)
	{
		ConsoleGlobals::m_helpMessages = new std::map<std::string, console_command_cb>();
	}

	if (ConsoleGlobals::m_commands->size() <= 0)
	{
		ConsoleGlobals::m_commands->insert(std::pair<std::string, console_command_cb>(true_command_name, callback));
		ConsoleGlobals::m_helpMessages->insert(std::pair<std::string, console_command_cb>(true_command_name, helpMessage));
		return true;
	}
	std::map<std::string, console_command_cb>::const_iterator alreadyExists = ConsoleGlobals::m_commands->find(true_command_name);
	if (alreadyExists != ConsoleGlobals::m_commands->end())
	{
		const std::string warning = command_name + " already exists.";
		DebuggerPrintf(warning.c_str());
		if (g_Console != nullptr)
		{
			g_Console->ConsolePrintf(WARNING, warning.c_str());
		}
		return false;
	}
	ConsoleGlobals::m_commands->insert(std::pair<std::string, console_command_cb>(true_command_name, callback));
	ConsoleGlobals::m_helpMessages->insert(std::pair<std::string, console_command_cb>(true_command_name, helpMessage));
	return true;
}

const bool Console::RunCommand(const std::string& command_line)
{
	m_ranCommand = false;
	Command newCommand = Command(command_line);
	for (std::map<std::string, console_command_cb>::iterator index = ConsoleGlobals::m_commands->begin(); index != ConsoleGlobals::m_commands->end(); index++)
	{
		if (newCommand.get_command_name() == index->first)
		{
			index->second(newCommand);
			m_ranCommand = true;
			return true;
		}
	}
	ConsolePrintf(URGENT, "Invalid command!");
	return false;
}

void Console::Render() const
{
	g_Renderer->DisableTexture();
	//float aspect = (assumedAspects.x / assumedAspects.y);

	float width = (float)g_Renderer->GetScreenWidth();
	float height = (float)g_Renderer->GetScreenHeight();
	g_Renderer->CullFaceTrueFalse(false);
	g_Renderer->DepthTestingTrueFalse(false);

	g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(width, height));
	ShaderProgram::SetProjectionAndViewAll(m_Projection, m_View);

	if (m_overlayMat != nullptr)
	{
		m_overlayMat->SetModelMatrix(m_GeneralModel);
		m_overlaymesh.DrawWith(m_overlayMat);
	}
	//g_Renderer->DrawVertexArray(overlayVertices, PRIMITIVE_QUADS, nullptr);
	RenderCommandPromptLines();
	RenderCurrentlyBeingTypedText();
	RenderCursor();
	g_Renderer->DisableTexture();
}


void Console::RenderCommandPromptLines() const
{
	if (m_consoleFont == nullptr)
	{
		return;
	}
	/*
	Mesh m_lineMesh;
	Mesh m_beingTypedMesh;
	Mesh m_cursorMesh;
	*/
	for (int pageIdx = 0; pageIdx < m_consoleFont->GetPageCount(); pageIdx++)
	{
		m_textMat[pageIdx].SetModelMatrix(m_GeneralModel);
		m_lineMesh[pageIdx].DrawWith(&m_textMat[pageIdx]);
	}
	//m_lineMesh.DrawWith(m_textMat);
	//std::vector<Texture*> fontPages;
	//m_consoleFont->RenderText(lineVertices);
}

void Console::UpdateLineText()
{
	lineVertices.clear();
	lineIndices.clear();
	if (m_consoleFont == nullptr)
	{
		return;
	}
	int pageCount = m_consoleFont->GetPageCount();
	lineVertices.resize(pageCount);
	lineIndices.resize(pageCount);
	float yStep = ((commandPromptBoxEnd.y - commandPromptBoxStart.y) / m_maxNumLines);
	Vector3 currentPosition;
	currentPosition.x = commandPromptBoxStart.x;
	currentPosition.y = commandPromptBoxStart.y;
	Vector3 curLinePos;
	curLinePos.x = commandLineStart.x + startOfInputLineTranslation.x;
	curLinePos.y = commandLineStart.y + startOfInputLineTranslation.y - 3.5f;
	std::vector<std::vector<Vertex_PCT>> verticesToAdd;
	std::vector<std::vector<unsigned int>> indicesToAdd;
	std::vector<unsigned int> numVerts;
	numVerts.resize(pageCount);
	for (int i = 0; i < pageCount; i++)
	{
		numVerts[i] = 0;
	}
	for (int lineIndex = currentFirstViewable; lineIndex >= 0 && lineIndex > (int)(currentFirstViewable - (m_maxNumLines)) && commandPromptLines.size() > 0; lineIndex--)
	{

		if (commandPromptLines[lineIndex].drawInBox == true)
		{
			float xWidth = m_consoleFont->CalcTextWidth(commandPromptLines[lineIndex].line, m_fontSize);
			float yHeight = m_consoleFont->CalcTextHeight(commandPromptLines[lineIndex].line, m_fontSize);
			AABB2 minsMaxs;
			minsMaxs.mins.x = currentPosition.x;
			minsMaxs.mins.y = currentPosition.y;
			minsMaxs.maxs.x = currentPosition.x + xWidth;
			minsMaxs.maxs.y = currentPosition.y + (yHeight);
			//g_Renderer->DrawAABB(minsMaxs, commandPromptLines[lineIndex].boxColor);
		}
		
		m_consoleFont->GetTextToRender(verticesToAdd, indicesToAdd, curLinePos, commandPromptLines[lineIndex].line, m_fontSize, s_rightDirection, s_upDirection, commandPromptLines[lineIndex].lineColor);
		m_lineMesh.resize(pageCount);
		m_textMat.resize(pageCount);
		for (int i = 0; i < pageCount; i++)
		{
			lineVertices[i].insert(lineVertices[i].end(), verticesToAdd[i].begin(), verticesToAdd[i].end());
			for (size_t indiceIdx = 0; indiceIdx < indicesToAdd[i].size(); indiceIdx++)
			{
				unsigned int idice = indicesToAdd[i][indiceIdx];
				lineIndices[i].push_back(idice + numVerts[i]);
			}
			numVerts[i] = lineVertices[i].size();
		}
		currentPosition.y += yStep;
		curLinePos.y += yStep;
	}

	for (int pageIdx = 0; pageIdx < pageCount; pageIdx++)
	{
		m_lineMesh[pageIdx].setVBO(lineVertices[pageIdx]);
		m_lineMesh[pageIdx].setIBO(lineIndices[pageIdx]);
		m_textMat[pageIdx].SetOrAddTexture(m_consoleFont->RequestPage(pageIdx), 0, "gDiffuseTex", 0);
		m_textMat[pageIdx].SetProgram(m_fontShader);
		const Texture* fontPage = m_consoleFont->GetPageForIdx(pageIdx);
		m_textMat[pageIdx].SetOrAddTexture(fontPage, 0, "gTextureDiffuse", 0);
	}
}

void Console::UpdatePresentlyBeingTypedText()
{
	beingTypedVertices.clear();
	beingTypedIndices.clear();
	if (m_consoleFont == nullptr)
	{
		return;
	}
	Vector3 startPos;
	startPos.x = inputBoxLineStart.x + inputBoxTranslation.x + startOfInputLineTranslation.x;
	startPos.y = inputBoxLineStart.y + inputBoxTranslation.y + startOfInputLineTranslation.y;
	startPos.z = 0.f;
	m_consoleFont->GetTextToRender(beingTypedVertices, beingTypedIndices,startPos, currentlyBeingTyped, 
		m_fontSize, s_rightDirection, s_upDirection, s_defaultColor);

	int pageCount = m_consoleFont->GetPageCount();
	m_beingTypedMesh.resize(pageCount);
	for (int pageIdx = 0; pageIdx < pageCount; pageIdx++)
	{
		m_beingTypedMesh[pageIdx].setVBO(beingTypedVertices[pageIdx]);
		m_beingTypedMesh[pageIdx].setIBO(beingTypedIndices[pageIdx]);
		m_textMat[pageIdx].SetOrAddTexture(m_consoleFont->RequestPage(pageIdx), 0, "gDiffuseTex", 0);
		m_textMat[pageIdx].SetProgram(m_fontShader);
		const Texture* fontPage = m_consoleFont->GetPageForIdx(pageIdx);
		m_textMat[pageIdx].SetOrAddTexture(fontPage, 0, "gTextureDiffuse", 0);
	}
}
void Console::RenderCurrentlyBeingTypedText() const
{
	if (m_consoleFont == nullptr)
	{
		return;
	}
	for (int pageIdx = 0; pageIdx < m_consoleFont->GetPageCount(); pageIdx++)
	{
		m_textMat[pageIdx].SetModelMatrix(m_GeneralModel);
		m_beingTypedMesh[pageIdx].DrawWith(&m_textMat[pageIdx]);
	}
}

void Console::UpdateCursorText()
{
	cursorVertices.clear();
	cursorIndices.clear();
	if (m_consoleFont == nullptr)
	{
		return;
	}
	Vector3 startPos;
	startPos.x = inputBoxLineStart.x - 0.04f + startOfInputLineTranslation.x;
	startPos.y = inputBoxLineStart.y + inputBoxTranslation.y + startOfInputLineTranslation.y;
	startPos.z = 0.f;
	if (currentlyBeingTyped.length() != 0 && cursorPosition != 0)
	{
		std::string substring = currentlyBeingTyped.substr(0, cursorPosition);
		float xToAdd = m_consoleFont->CalcTextWidth(substring, m_fontSize);
		startPos.x += xToAdd;
	}
	m_consoleFont->GetTextToRender(cursorVertices, cursorIndices, startPos, s_brokenBar, m_fontSize, 
		s_rightDirection, s_upDirection, s_defaultColor);

	int pageCount = m_consoleFont->GetPageCount();
	m_cursorMesh.resize(pageCount);
	for (int pageIdx = 0; pageIdx < pageCount; pageIdx++)
	{
		m_cursorMesh[pageIdx].setVBO(cursorVertices[pageIdx]);
		m_cursorMesh[pageIdx].setIBO(cursorIndices[pageIdx]);
		m_textMat[pageIdx].SetOrAddTexture(m_consoleFont->RequestPage(pageIdx), 0, "gDiffuseTex", 0);
		m_textMat[pageIdx].SetProgram(m_fontShader);
		const Texture* fontPage = m_consoleFont->GetPageForIdx(pageIdx);
		m_textMat[pageIdx].SetOrAddTexture(fontPage, 0, "gTextureDiffuse", 0);
	}
}
void Console::RenderCursor() const
{
	if (m_consoleFont == nullptr || m_cursorOn == false)
	{
		return;
	}
	for (int pageIdx = 0; pageIdx < m_consoleFont->GetPageCount(); pageIdx++)
	{
		m_textMat[pageIdx].SetModelMatrix(m_GeneralModel);
		m_cursorMesh[pageIdx].DrawWith(&m_textMat[pageIdx]);
	}
	//m_consoleFont->RenderText(cursorVertices);
	//Vector3 startPos;
	//startPos.x = inputBoxLineStart.x - 0.04f;
	//startPos.y = inputBoxLineStart.y;
	//startPos.z = 0.f;
	//if (currentlyBeingTyped.length() == 0 || cursorPosition == 0)
	//{
	//	m_consoleFont->RenderText(startPos, s_brokenBar, m_fontSize, s_rightDirection, s_upDirection, s_defaultColor);
	//	return;
	//}
	//std::string substring = currentlyBeingTyped.substr(0, cursorPosition);
	//float xToAdd = m_consoleFont->CalcTextWidth(substring, m_fontSize);
	//startPos.x += xToAdd;
	//m_consoleFont->RenderText(startPos, s_brokenBar, m_fontSize, s_rightDirection, s_upDirection, s_defaultColor);
}


const bool Console::IsQuitting() const
{
	return m_quitApplication;
}

const size_t Console::GetNumberOfCommandPromptLines() const
{
	return commandPromptLines.size();
}

const bool Console::GetRanCommand() const
{
	return m_ranCommand;
}

const std::vector<std::string> Console::GetMessages(size_t start, size_t end)
{
	if (start > end)
	{
		return std::vector<std::string>();
	}
	if (start >= commandPromptLines.size())
	{
		start = commandPromptLines.size() - 1;
	}
	if (end >= commandPromptLines.size())
	{
		end = commandPromptLines.size() - 1;
	}
	std::vector<std::string> messages;
	for (size_t i = start; i <= end; i++)
	{
		messages.push_back(commandPromptLines.at(i).line);
	}
	return messages;
}