#ifndef _EDITOR_H
#define _EDITOR_H

#include "../../YoAudio/include/Timer.h"
#include "InputManager.h"
#include "GUI.h"

class Editor
{
private:
	static Editor* sInstance;
	
	const int FRAME_RATE = 30;

	bool mQuit;
	Timer* mTimer;
	InputManager* mInputManager;
	Gui* mUi;

	SDL_Event mEvents;
public:
	static Editor* GetInstance();
	static void Release();

	void Run();
private:
	Editor();
	~Editor();

	void App();
};

#endif // !_EDITOR_H
