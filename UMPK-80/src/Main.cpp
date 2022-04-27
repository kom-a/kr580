#include <iostream>

#include <Emulator.h>

#include "Window.h"
#include "ViewsController.h"

void Melody1FunctionProc(KR580VM80A* emu)
{

}

void Melody2FunctionProc(KR580VM80A* emu)
{

}

void DisplayProc(KR580VM80A* emu)
{
	ViewsController& views_controller = ViewsController::GetInstance();
	StandToolsView* stand_tools = views_controller.GetStandToolsView();

	stand_tools->Display(&emu->Memory[0x0BFA]);
}

WORD GetDisplayRepresentation(WORD value)
{
	switch (value)
	{
		case 0x0: return 0x3f;
		case 0x1: return 0x06;
		case 0x2: return 0x5b;
		case 0x3: return 0x4f;
		case 0x4: return 0x66;
		case 0x5: return 0x6D;
		case 0x6: return 0x7D;
		case 0x7: return 0x07;
		case 0x8: return 0x7F;
		case 0x9: return 0x6F;
		case 0xA: return 0x77;
		case 0xB: return 0x7C;
		case 0xC: return 0x39;
		case 0xD: return 0x5E;
		case 0xE: return 0x79;
		case 0xF: return 0x71;
	}
}

void DecodeDisplayProc(KR580VM80A* emu)
{
	for (int i = 0; i < 6; i++)
		emu->Memory[0x0BFA + i] = GetDisplayRepresentation(emu->Memory[0x0BF0 + i]);
}

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	emu->SetBuiltInFunction(0x05B0, Melody1FunctionProc);
	emu->SetBuiltInFunction(0x05BA, Melody2FunctionProc);
	emu->SetBuiltInFunction(0x01C8, DisplayProc);
	emu->SetBuiltInFunction(0x01E9, DecodeDisplayProc);

	ViewsController& views_controller = ViewsController::GetInstance();
	views_controller.SetToolbarView(new ToolbarView());
	views_controller.SetEditorView(new EditorView());
	views_controller.SetMemoryView(new MemoryView(emu->Memory + USER_MEMORY_OFFSET, USER_MEMORY_SIZE));
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
