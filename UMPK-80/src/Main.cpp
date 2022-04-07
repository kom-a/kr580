#include <iostream>

#include <Emulator.h>

#include "Window.h"
#include "Views.h"

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	window->Add(new Toolbar());
	window->Add(new MemoryView());
	window->Add(new StackView());
	window->Add(new EditorView());
	window->Add(new RegistersView());
	window->Add(new InPortView());
	window->Add(new OutPortView());
	window->Add(new StandToolsView());

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
