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

int main()
{
	Window* window = new Window(1280 / 1.5f, 720 / 1.5f, "UMPK-80");
	KR580VM80A* emu = new KR580VM80A();

	std::vector<WORD> program = {
		LDA_a16, 0x00, 0x09,
		MOV_B_A,
		LDA_a16, 0x01, 0x09,
		ADD_B,
		STA_a16, 0x02, 0x09,
		JMP_a16, 0x00, 0x08
	};

	std::string source = "mvi a, 02\n"
		"mvi b, 03\n"
		"add b\n"
		"sta1 08FB\n"
		"rst1";
	Compiler compiler;

	source += '\n';

	compiler.Compile(source);
	if (compiler.errorOccured)
	{
		for (auto error : compiler.compileErrors.messages)
		{
			std::cout << "line: " << std::get<0>(error) << " " << std::get<1>(error);
		}
		return -1;
	}

	program = compiler.resultBinary;
	
	emu->LoadProgram(program);

	/*window->Add(new Registers(emu));
	window->Add(new Tools(emu));*/

	window->Add(new Toolbar());
	window->Add(new MemoryView());
	window->Add(new StackView());
	window->Add(new EditorView());
	window->Add(new RegistersView());

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