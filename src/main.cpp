#include <stdio.h>
#include "Editor.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	Editor* editor = Editor::GetInstance();

	editor->Run();

	Editor::Release();
	editor = nullptr;

	return 0;
}
