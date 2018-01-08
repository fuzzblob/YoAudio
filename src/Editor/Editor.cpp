#include "Editor.h"

GameManager* Editor::game = nullptr;

Editor::Editor()
{
	//game = GameManager::GetInstance();

	Graphics::GuiRun();

	//game->Run();
}

Editor::~Editor()
{
	//GameManager::Release();
	//game = nullptr;
}
