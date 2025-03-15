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
	mKeyboardStates = nullptr;
}

InputManager::~InputManager()
{

}
