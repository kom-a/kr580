#include "MemoryView.h"

MemoryView::MemoryView() 
	: m_MemoryEditor()
{
	m_MemoryEditor.Cols = 1;
	m_MemoryEditor.OptShowAscii = false;
	m_MemoryEditor.OptShowOptions = false;
}

MemoryView::~MemoryView()
{

}

void MemoryView::Render(KR580VM80A* emu)
{
	if (!m_MemoryEditor.Open)
		return;

	ImGui::Begin("Memory");

	m_MemoryEditor.DrawContents(emu->Memory + USER_MEMORY_OFFSET, USER_MEMORY_SIZE, USER_MEMORY_OFFSET);

	ImGui::End();
}
