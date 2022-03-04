#include "StackView.h"

StackView::StackView()
	: m_MemoryEditor()
{
	m_MemoryEditor.Cols = 1;
}

StackView::~StackView()
{

}

void StackView::Render(KR580VM80A* emu)
{
	if (m_MemoryEditor.Open)
		m_MemoryEditor.DrawWindow("Stack", emu->Memory + STACK_OFFSET, STACK_SIZE, STACK_OFFSET);
}
