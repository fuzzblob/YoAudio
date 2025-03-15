#include "Editor.h"

// have to override SDL defined main for some reson...
#undef main
int main(void)
{
	auto editor = std::make_unique<YoaEditor::Editor>();
	return editor->Run();
}
