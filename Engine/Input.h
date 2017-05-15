
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <DirectXMath.h>
#include <dinput.h>
#include <Windows.h>
#include <queue>
using namespace DirectX;

class KeyEvent
{
public:
	enum EventType
	{
		Press,
		Release,
		NULLKey
	};
	KeyEvent(EventType type, unsigned char keycode);
	unsigned char getCode();
	bool isPress();
	bool isRelease();
private:
	EventType type;
	unsigned char keyCode;
};


class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void init(HWND hwnd );
	void update();
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
	void lockCursor(bool);
	void showCursor(bool);
	XMFLOAT2 getCusorDelta();
	XMFLOAT2 getMouseSensivity();
	void setMouseSensivity(XMFLOAT2 mouseSensivity);

	void onKeyPressed(unsigned char keycode);

	void onKeyReleased(unsigned char keycode);

	KeyEvent readKey();
	KeyEvent peakKey();
	bool isEmpty();
	std::vector  < KeyEvent > getKeyBuffer();

	void clear();


private:
	std::vector  < KeyEvent > keyBuffer;

	bool m_keys[256];
	HWND m_hwnd;
	bool m_isCursorLock = true;
	bool m_isCursorShow = true;
	bool isFocused = false;
	XMFLOAT2 m_cursorOldPos;
	XMFLOAT2 m_cursorDelta;
	XMFLOAT2 m_mouseSensivity;
};