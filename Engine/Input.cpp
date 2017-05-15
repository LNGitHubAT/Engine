////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Input.h"


Input::Input()
{
}


Input::Input(const Input& other)
{
}


Input::~Input()
{
}


void Input::init(HWND hwnd)
{
	int i;
	

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	this->m_hwnd = hwnd;

	m_mouseSensivity = XMFLOAT2(1.0f, 1.0f);
	m_cursorDelta = XMFLOAT2(0.0f, 0.0f);
	return;
	
	
}

void Input::update()
{
	POINT currentCursorPos;

	GetCursorPos(&currentCursorPos);
	if (GetFocus() == m_hwnd)
	{
		ShowCursor(m_isCursorShow);
		if (!isFocused)
		{
			isFocused = true;
			if (m_isCursorLock)
			{
				m_cursorOldPos.x = currentCursorPos.x;
				m_cursorOldPos.y = currentCursorPos.y;
			}
		}
		m_cursorDelta.x = currentCursorPos.x - m_cursorOldPos.x;
		m_cursorDelta.y = currentCursorPos.y - m_cursorOldPos.y;

		if (m_isCursorLock)
		{
			SetCursorPos(m_cursorOldPos.x, m_cursorOldPos.y);
		}
	}
	else
	{
		isFocused = false;
	}
}


void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	if (!m_keys[input])
	{
		onKeyPressed((char)input);
	}
	m_keys[input] = true;
	return;
}


void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	if (m_keys[input])
	{
		onKeyReleased((char)input);
	}
	m_keys[input] = false;
	return;
}


bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

void Input::lockCursor(bool isLock)
{
	m_isCursorLock = isLock;
}

void Input::showCursor(bool isShow)
{
	m_isCursorShow = isShow;
}

XMFLOAT2 Input::getCusorDelta()
{
	return m_cursorDelta;
}

XMFLOAT2 Input::getMouseSensivity()
{
	return m_mouseSensivity;
}

void Input::setMouseSensivity(XMFLOAT2 mouseSensivity)
{
	m_mouseSensivity = mouseSensivity;
}

void Input::onKeyPressed(unsigned char keycode)
{
	keyBuffer.push_back(KeyEvent(KeyEvent::Press, keycode));
}

void Input::onKeyReleased(unsigned char keycode)
{
	keyBuffer.push_back(KeyEvent(KeyEvent::Release, keycode));
}

KeyEvent Input::readKey()
{
	if (keyBuffer.size() > 0)
	{
		KeyEvent e = keyBuffer.front();
		//keyBuffer.pop();
		return e;
	}
	else
	{
		return KeyEvent(KeyEvent::NULLKey, 0);
	}
}

KeyEvent Input::peakKey()
{
	if (keyBuffer.size() > 0)
	{
		KeyEvent e = keyBuffer.front();
		//keyBuffer.pop();
		return e;
	}
	else
	{
		return KeyEvent(KeyEvent::NULLKey, 0);
	}
}

KeyEvent::KeyEvent(KeyEvent::EventType type, unsigned char keycode)
	:
	type(type),
	keyCode(keycode)
{
}

unsigned char KeyEvent::getCode()
{
	return keyCode;
}

bool KeyEvent::isPress()
{
	return type == Press;
}

bool KeyEvent::isRelease()
{
	return type = Release;
}

bool Input::isEmpty()
{
	return keyBuffer.empty();
}

std::vector<KeyEvent> Input::getKeyBuffer()
{
	return keyBuffer;
}

void Input::clear()
{
	keyBuffer.clear();
}

