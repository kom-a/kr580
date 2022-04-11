#pragma once

#include "View.h"

#include <imgui/imgui.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

class MemoryView : public View
{
public:
	MemoryView();
	~MemoryView();

	void Render(KR580VM80A* emu);

private:
	void DrawContent();
	void CalcSizes();

private:
	MemoryEditor m_MemoryEditor;
};
