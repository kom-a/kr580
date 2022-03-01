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

#include "Window.h"
#include "Views.h"

int main()
{
	Window* window = new Window(640, 480, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	std::vector<WORD> program = {
		MVI_C_d8, 0x08,
		LXI_H_d16, 0x00, 0x09,
		LXI_D_d16, 0x00, 0x0A,
		MOV_A_M,
		SUI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x08, 0x08,
		MVI_C_d8, 0x08,
		MOV_A_M,
		ADI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x14, 0x08,
		RST_1
	};

	emu->LoadProgram(program);

	window->Add(new MemEditor(emu));
	window->Add(new Registers(emu));
	window->Add(new Tools(emu));

	while (!window->Closed())
	{
		window->Render();
		window->Update();
	}

	// delete window;
	// delete emu;

	return 0;
}