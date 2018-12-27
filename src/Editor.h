#ifndef _EDITOR_H
#define _EDITOR_H

#include <memory>

class Editor
{
private:
	bool mQuit = false;

	void App();
	void Menu();
public:
	void Run();
};

#endif // !_EDITOR_H
