#pragma once

#include "View.h"

#include <imgui/imgui.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

class OutPortView : public View
{
public:
	OutPortView();
	~OutPortView();

	void Render(KR580VM80A* emu);

private:
	MemoryEditor m_Editor;
};
