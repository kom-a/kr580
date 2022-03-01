#include "MemEditor.h"

#include <imgui/imgui.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>

MemEditor::MemEditor(KR580VM80A* emu)
	: View(emu)
{
}

void MemEditor::Render()
{
	static MemoryEditor mem_edit_1;
	mem_edit_1.DrawWindow("Memory Editor", m_Emulator->Memory, MEMORY_SIZE);
}
