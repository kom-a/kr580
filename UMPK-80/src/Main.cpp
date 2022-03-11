#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

#include <Emulator.h>
#include "../Shared/Include/ISA.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Window.h"
#include "Views.h"

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	std::vector<WORD> program = {
		IN_pp, 0x05,
		MOV_B_A,
		ANI_d8, 0x82,
		JNZ_a16, 0x1A, 0x08,
		MOV_A_B, 
		ANI_d8, 0x20,
		JZ_a16, 0x1A, 0x08,
		MOV_A_B,
		ANI_d8, 0xBF,
		ORI_d8, 0x10,
		XRI_d8, 0x04,
		OUT_pp, 0x05,
		JMP_a16, 0x00, 0x08,
		MOV_A_B,
		ANI_d8, 0xFE,
		ORI_d8, 0x08,
		XRI_d8, 0x02,
		OUT_pp, 0x05,
		JMP_a16, 0x00, 0x08
	};

	emu->LoadProgram(program);

	/*window->Add(new Registers(emu));
	window->Add(new Tools(emu));*/

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
