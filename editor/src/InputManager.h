#pragma once
#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL.h>

class InputManager
{
private:
	const Uint8* mKeyboardStates = nullptr;
public:
	bool KeyDown(SDL_Scancode scanCode);
	void Update();
	InputManager();
	~InputManager();
};

#endif // !_INPUTMANAGER_H
