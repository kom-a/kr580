#pragma once

#include "View.h"

#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

class StackView : public View
{
public:
	StackView();
	~StackView();

	void Render(KR580VM80A* emu);

private:
	MemoryEditor m_MemoryEditor;
};
