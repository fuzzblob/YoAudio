#include "InputManager.h"

InputManager* InputManager::sInstance = nullptr;

InputManager * InputManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new InputManager();
	}

	return sInstance;
}

void InputManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

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
