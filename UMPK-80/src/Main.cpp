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
		STA_a16, 0x02, 0x09,
		JMP_a16, 0x00, 0x08
	};

	std::string str;
	Compiler compiler;
	std::ifstream is("C:\\Users\\en9er\\OneDrive\\Рабочий стол\\test.txt");
	readfile(str, is);

	compiler.Compile(str, 0x0800);
	if (compiler.errorOccured)
	{
		for (auto error : compiler.compileErrors.messages)
		{
			std::cout << "line: " << std::get<0>(error) << " " << std::get<1>(error) << std::endl;
		}
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