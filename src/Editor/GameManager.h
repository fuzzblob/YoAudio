#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include "../YOA/Timer.h"
#include "InputManager.h"
#include "Graphics.h"

class GameManager
{
private:
	static GameManager* sInstance;
	
	const int FRAME_RATE = 30;

	bool mQuit;
	Timer* mTimer;
	Graphics* mGraphics;
	InputManager* mInputManager;

	SDL_Event mEvents;
public:
	static GameManager* GetInstance();
	static void Release();

	void Run();
private:
	GameManager();
	~GameManager();
};

#endif // !_GAMEMANAGER_H
