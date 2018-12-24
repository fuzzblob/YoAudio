#ifndef _EDITOR_H
#define _EDITOR_H

#include <memory>
#include "Timer.h"

class Editor
{
private:
	bool mQuit = false;
	std::unique_ptr<Timer> mTimer;

	void App();
	void Menu();
public:
	void Run();
};

#endif // !_EDITOR_H
