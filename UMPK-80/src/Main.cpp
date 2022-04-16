#include <iostream>

#include <Emulator.h>

#include "Window.h"
#include "Views.h"

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	window->SetToolbarView(new ToolbarView());
	window->SetMemoryView(new MemoryView());
	window->SetStackView(new StackView());
	window->SetEditorView(new EditorView());
	window->SetRegistersView(new RegistersView());
	window->SetInPortView(new InPortView());
	window->SetOutPortView(new OutPortView());
	window->SetStandToolsView(new StandToolsView());

	while (!window->Closed())
	{
		if (emu->Running)
			emu->Step();

		window->Render(emu);
		window->Update();
	}

	// delete window;
	// delete emu;

	return 0;
}
