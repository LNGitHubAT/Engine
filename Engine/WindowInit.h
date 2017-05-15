#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>


#include "Input.h"
#include "Timer.h"
#include "Game.h"
class WindowsInit
{
public:
	WindowsInit();
	WindowsInit(const WindowsInit&);
	~WindowsInit();

	bool init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Update();
	bool render();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	Input* m_Input;
	Game* m_game;
	Timer* timer;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static WindowsInit* instance = 0;