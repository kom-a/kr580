#include <iostream>

#include <Emulator.h>

#include "Window.h"
#include "ViewsController.h"

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();
	
	ViewsController& views_controller = ViewsController::GetInstance();
	views_controller.SetToolbarView(new ToolbarView());
	views_controller.SetEditorView(new EditorView());
	views_controller.SetMemoryView(new MemoryView());
	views_controller.SetStackView(new StackView());
	views_controller.SetRegistersView(new RegistersView());
	views_controller.SetInPortView(new InPortView());
	views_controller.SetOutPortView(new OutPortView());
	views_controller.SetStandToolsView(new StandToolsView());
	
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
