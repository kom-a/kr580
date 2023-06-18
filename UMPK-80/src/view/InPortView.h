#pragma once

#include "View.h"

#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

class InPortView : public View
{
public:
	InPortView();
	~InPortView();

	void Render(KR580VM80A* emu);

private:
	MemoryEditor m_Editor;
};
