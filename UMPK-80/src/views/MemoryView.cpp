#include "MemoryView.h"

MemoryView::MemoryView() 
	: m_MemoryEditor()
{
	m_MemoryEditor.Cols = 1;
}

MemoryView::~MemoryView()
{

}

void MemoryView::Render(KR580VM80A* emu)
{
	if(m_MemoryEditor.Open)
		m_MemoryEditor.DrawWindow("Memory Editor", emu->Memory, MEMORY_SIZE);
}
