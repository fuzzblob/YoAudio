#include "Editor.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	std::unique_ptr<Editor> editor = std::make_unique<Editor>();
	return editor->Run();
}
