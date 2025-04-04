#pragma once
#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL.h>

namespace YoaEditor
{
	class InputManager
	{
	private:
		const Uint8* mKeyboardStates = nullptr;
	public:
		bool KeyDown(SDL_Scancode scanCode) const;
		void Update();
		InputManager() { };
		~InputManager() { };
	};
}  // namespace YoaEditor
#endif // !_INPUTMANAGER_H
