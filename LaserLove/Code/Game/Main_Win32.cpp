#include <windows.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "Game/TheApp.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Utils/AllocationsConsoleCommands.hpp"
#include "Engine/Utils/Allocations.hpp"
#include "Engine/Utils/Logger.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

/*
NOTE: 
for whatever reason upon running, at least on my computer, it appears as though the .exe file will open but the focus will try to remain on
Visual Studio 2013.  There appears to be no errors or warnings that appear to hint towards why this is... So for the time being
if you encounter this, just click on VS2013, then on the .exe file presently running (it should be visible on the window's toolbar),
and you'll be able to see and interact with protogame.
*/


//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
const int OFFSET_FROM_WINDOWS_DESKTOP = 50;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = 1600.0;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = VIEW_RIGHT * static_cast<double>(TheGame::WINDOW_PHYSICAL_HEIGHT) / static_cast<double>(TheGame::WINDOW_PHYSICAL_WIDTH);


bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Win32 OpenGL Test App";


//-----------------------------------------------------------------------------------------------
//UINT = unsigned int.
LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	unsigned char asKey = (unsigned char)wParam;
	switch (wmMessageCode)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		g_isQuitting = true;
		return 0;


	case WM_LBUTTONDOWN:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(VK_LBUTTON, true);
		}
		break;
	case WM_RBUTTONDOWN:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(VK_RBUTTON, true);
		}
		break;
	case WM_LBUTTONUP:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(VK_LBUTTON, false);
		}
		break;
	case WM_RBUTTONUP:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(VK_RBUTTON, false);
		}
		break;

	case WM_KEYDOWN:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(asKey, true);
		}
		//		g_keyDownArray[ asKey ] = true;
		break;

	case WM_KEYUP:
		if (g_isQuitting == false && g_input != nullptr)
		{
			g_input->setKeyDownStatus(asKey, false);
		}
		//		g_keyDownArray[ asKey ] = false;
		break;
	case WM_SETFOCUS:
	{
		if (g_TheApp != nullptr)
		{
			g_TheApp->OnGainedFocus();
		}
		break;
	}
	case WM_KILLFOCUS:
	{
		if (g_TheApp != nullptr)
		{
			g_TheApp->OnLostFocus();
		}
		 break;
	}

	case WM_MOUSEWHEEL:
		if (g_input == nullptr)
		{
			break;
		}
		short scrollDir = GET_WHEEL_DELTA_WPARAM(wParam);
		if (scrollDir > 0)
		{
			g_input->setWheelScroll(true, false);
		}
		else if (scrollDir < 0)
		{
			g_input->setWheelScroll(false, true);
		}
		break;
	}

	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow(HINSTANCE applicationInstanceHandle)
{
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

	g_Renderer->CreateOpenGLWindow(applicationInstanceHandle, APP_NAME, OFFSET_FROM_WINDOWS_DESKTOP, TheGame::WINDOW_PHYSICAL_WIDTH, TheGame::WINDOW_PHYSICAL_HEIGHT,
		g_hWnd, g_displayDeviceContext, g_openGLRenderingContext, VIEW_LEFT, VIEW_RIGHT, VIEW_TOP, VIEW_BOTTOM, windowClassDescription);
}


//-----------------------------------------------------------------------------------------------
void RunMessagePump()
{
	MSG queuedMessage;
	for (;;)
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}


//-----------------------------------------------------------------------------------------------
void Update()
{
	Profiler::ProfileFrameMark();
	static double s_timeLastFrameBegan = GetCurrentTimeSeconds(); // "static" local variables are initialized once, when first encountered, thus this function call only happens once
	double timeThisFrameBegan = GetCurrentTimeSeconds();
	float deltaSeconds = static_cast<float>(timeThisFrameBegan - s_timeLastFrameBegan);
	s_timeLastFrameBegan = timeThisFrameBegan;
	g_TheApp->Update(deltaSeconds);

	if (g_TheGame->isQuitting() == true)
	{
		g_isQuitting = true;
	}
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	g_TheApp->Render();

	SwapBuffers(g_displayDeviceContext);
}


//-----------------------------------------------------------------------------------------------
void RunFrame()
{
	RunMessagePump();
	Update();
	Render();
}


//-----------------------------------------------------------------------------------------------
void Initialize(HINSTANCE applicationInstanceHandle)
{
	g_Console = nullptr;
	g_input = new Input(0, 1);
	g_input->activateDPIAwareness(); // this tells Windows not to lie about mouse position and input.
	g_debugRenderer = new DebugRenderring();
	g_TheApp = new TheApp();
	CreateOpenGLWindow(applicationInstanceHandle);
	g_Console = new Console();
	Profiler::RegisterProfilerConsoleCommands();
	g_Renderer = new Renderer();
	g_TheApp->SetUpFrameBuffer();
	AllocationsConsoleCommands::AddConsoleCommands();

	//SetUpFrameBuffer();
	Font* theFont = Font::CreateOrGetFont("Data/Font/", "DebugFont.fnt");
	g_TheApp->m_font = theFont;
	g_Console->ChangeFont(theFont);

	g_TheGame = new TheGame();
	JobSystem::StartupJobSystem(-1);
}


//-----------------------------------------------------------------------------------------------
void Shutdown()
{
	JobSystem::ShutdownJobSystem();
	delete g_TheGame;
	delete g_TheApp;
	delete g_Renderer;
	delete g_input;
	delete g_debugRenderer;
	delete g_Console;
	g_TheGame = nullptr;
	g_TheApp = nullptr;
	g_Renderer = nullptr;
	g_input = nullptr;
	g_debugRenderer = nullptr;
	g_Console = nullptr;
	Texture::DeleteAllTextures();
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int)
{
	UNUSED(commandLineString);
	Logger::GlobalStart();
	Allocations::MemoryAnalyticsStartup();
	Initialize(applicationInstanceHandle);
	Profiler::ProfilerSystemStartup();
	while (!g_isQuitting)
	{
		RunFrame();
	}
	Profiler::ProfilerSystemsShutdown();
	Shutdown();
	Allocations::MemoryAnalyticsShutdown();
	Logger::GlobalEnd();
	return 0;
}


