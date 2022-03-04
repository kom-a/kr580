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
		MVI_A_d8, 0x00,
		MVI_H_d8, 0x00,
		MVI_L_d8, 0x00,
		MOV_M_A,
		INR_A,
		INX_H,
		MOV_B_A,
		MOV_A_H,
		CPI_d8, 0x08,
		MOV_A_B,
		JNZ_a16, 0x06, 0x08,
		
		RST_1
	};
	
	emu->LoadProgram(program);

	/*window->Add(new Registers(emu));
	window->Add(new Tools(emu));*/

	window->Add(new MemoryView());

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