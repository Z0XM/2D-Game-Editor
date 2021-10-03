#include "src/Editor.hpp"

int main()
{
	Editor editor;
	while (editor.isRunning())
	{
		editor.update();
		editor.render();
	}
}