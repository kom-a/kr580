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

#include "src/Compiler.h"


void readfile(std::string& s, std::ifstream& is) {
	s.erase();
	if (is.bad()) return;
	//
	// attempt to grow string buffer to match file size,
	// this doesn't always work...
	s.reserve(is.rdbuf()->in_avail());
	char c;
	while (is.get(c))
	{
		// use logarithmic growth stategy, in case
		// in_avail (above) returned zero:
		if (s.capacity() == s.size())
			s.reserve(s.capacity() * 3);
		s.append(1, c);
	}
}


int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	std::vector<WORD> program = {

		LDA_a16, 0x00, 0x09,
		MOV_B_A,
		LDA_a16, 0x01, 0x09,
		ADD_B,
		MOV_B_A,
		JC_a16, 0x1D, 0x08,
		LDA_a16, 0x02, 0x09,
		MOV_C_A,
		MOV_A_B,
		SUB_C,
		JC_a16, 0x39, 0x08,
		SBI_d8, 0xF7,
		JC_a16, 0x39, 0x08,
		JMP_a16, 0x3E, 0x08,
		LDA_a16, 0x02, 0x09,
		MOV_C_A,
		MOV_A_B,
		SUB_C,
		MOV_B_A,
		JC_a16, 0x30, 0x08,
		MVI_A_d8, 0x01,
		SUB_B,
		JC_a16, 0x39, 0x08,
		JMP_a16, 0x3E, 0x08,
		MOV_A_B,
		SUI_d8, 0xF7,
		JC_a16, 0x39, 0x08,
		JMP_a16, 0x3E, 0x08,
		MVI_A_d8, 0x70,
		OUT_pp, 0x05,
		RST_1,
		MVI_A_d8, 0x07,
		OUT_pp, 0x05,
		RST_1
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
