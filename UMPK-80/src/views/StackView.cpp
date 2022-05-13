#include "StackView.h"

StackView::StackView()
	: m_MemoryEditor()
{
	m_MemoryEditor.Cols = 1;
	m_MemoryEditor.OptShowAscii = false;
	m_MemoryEditor.OptShowOptions = false;
}

StackView::~StackView()
{

}

void StackView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Stack", &m_Open);

	m_MemoryEditor.DrawContents(emu->Memory + STACK_OFFSET, STACK_SIZE + 1, STACK_OFFSET, true, "SP -> ", emu->SP);

	ImGui::End();
}
