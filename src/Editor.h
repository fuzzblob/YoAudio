#ifndef _EDITOR_H
#define _EDITOR_H

#include <memory>
#include "../../YoAudio/include/Timer.h"
#include "InputManager.h"
#include "GUI.h"

class Editor
{
private:
	const int FRAME_RATE = 30;
	bool m_quit = false;

	void App();
	void Menu();
public:
	void Run();
};

#endif // !_EDITOR_H
