#include "InputManager.h"

namespace YoaEditor
{
	bool InputManager::KeyDown(SDL_Scancode scanCode) const
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
}