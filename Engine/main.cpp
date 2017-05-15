////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "WindowInit.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	WindowsInit* window;
	bool result;
	
	
	// Create the system object.
	window = new WindowsInit();
	if(!window)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = window->init();
	if(result)
	{
		window->Run();
	}

	// Shutdown and release the system object.
	window->Shutdown();
	delete window;
	window = 0;

	return 0;
}