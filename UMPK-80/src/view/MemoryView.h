#pragma once

#include "View.h"

#include <imgui/imgui.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

class MemoryView : public View
{
public:
	MemoryView(uint8_t* memory, size_t memory_size);
	~MemoryView();

	void Render(KR580VM80A* emu);
	bool LoadFromFile(const std::string& filename);
	bool SaveToFile(const std::string& filename);

private:
	MemoryEditor m_MemoryEditor;
	uint8_t* m_Memory;
	size_t m_MemorySize;
};
