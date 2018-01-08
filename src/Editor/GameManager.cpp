#include "GameManager.h"

GameManager* GameManager::sInstance = nullptr;

GameManager * GameManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new GameManager();
	}

	return sInstance;
}

void GameManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

GameManager::GameManager()
{
	mQuit = false;

	mTimer = new Timer();
	
	mInputManager = InputManager::GetInstance();
	mGraphics = Graphics::GetInstance();

	if (Graphics::IsInitialized() == false)
	{
		mQuit = true;
	}
}

GameManager::~GameManager()
{
	Graphics::Release();
	mGraphics = nullptr;

	InputManager::Release();
	mInputManager = nullptr;

	delete mTimer;
	mTimer = nullptr;

	SDL_Quit();
}

void GameManager::Run()
{
	while (mQuit == false)
	{
		mTimer->Update();
		
		while (SDL_PollEvent(&mEvents) != 0)
		{
			if (mEvents.type == SDL_QUIT)
			{
				mQuit = true;
			}
		}

		if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE)
		{
			//printf("DeltaTime: %f\n", mTimer->DeltaTime());
			mInputManager->Update();
			//mGraphics->ClearBackBuffer();
			
			mGraphics->GuiRun();

			//mGraphics->Render();
			mTimer->Reset();
		}
	}
}
