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
	void App();
public:
	void Run();
};

#endif // !_EDITOR_H
