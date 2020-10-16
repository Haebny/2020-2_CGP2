#include "inputclass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	int i;

	// Initialize all the keys to being released and not pressed.
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	num1 = true;
	num2 = true;
	num3 = true;

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

void InputClass::SetKeyState(int key)
{
	switch (key)
	{
	case 1:
	{
		num1 = !num1;
		break;
	}
	case 2:
	{
		num2 = !num2;
		break;
	}
	case 3:
	{
		num3 = !num3;
		break;
	}
	default:
		break;
	}

	return;
}

bool InputClass::GetKeyState(int key)
{
	bool keyState;

	switch (key)
	{
	case 1:
	{
		return num1;
		break;
	}
	case 2:
	{
		return num2;
		break;
	}
	case 3:
	{
		return num3;
		break;
	}
	default:
		break;
	}
}