#include "InputManager.h"

bool InputManager::KeyDown(SDL_Scancode scanCode)
{
	return mKeyboardStates[scanCode];
}

void InputManager::Update()
{
	mKeyboardStates = SDL_GetKeyboardState(nullptr);
}

InputManager::InputManager()
{
	Uint8* mKeyboardStates = nullptr;
}

InputManager::~InputManager()
{

}
