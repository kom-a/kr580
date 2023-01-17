#pragma once

#include "View.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Compiler.h"

class ControlView : public View
{
public:
	ControlView();
	~ControlView();

	void Render(KR580VM80A* emu);

private:
	Compiler m_Compiler;
	Disassembler m_Disassembler;
};