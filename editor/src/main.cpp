#include "Editor.h"
#include <iostream>

// TODO(maris): check if sdl main still needs to be overridden
// have to override SDL defined main for some reson...
#undef main
int main(void)
{
    std::cout << PROJECT_NAME << " initializing. version: " << PROJECT_VERSION << "\n";
	auto editor = std::make_unique<YoaEditor::Editor>();
	return editor->Run();
}
