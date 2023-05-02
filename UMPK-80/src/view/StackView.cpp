#include "StackView.h"

StackView::StackView()
	: m_MemoryEditor()
{
	m_MemoryEditor.Cols = 1;
	m_MemoryEditor.OptShowAscii = false;
	m_MemoryEditor.OptShowOptions = true;
	m_MemoryEditor.OptReverse = true;
}

StackView::~StackView()
{

}

void StackView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Stack", &m_Open);

	m_MemoryEditor.DrawContents(emu->Memory + KR580VM80A::STACK_OFFSET, KR580VM80A::STACK_SIZE + 1, KR580VM80A::STACK_OFFSET, &emu->SP, "SP -> ");

	ImGui::End();
}
