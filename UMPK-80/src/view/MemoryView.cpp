#include "MemoryView.h"

#include <fstream>


MemoryView::MemoryView(uint8_t* memory, size_t memory_size)
	: m_Memory(memory),
	m_MemorySize(memory_size)
{
	m_MemoryEditor.Cols = 1;
	m_MemoryEditor.OptShowAscii = true;
	m_MemoryEditor.OptShowOptions = false;
}

MemoryView::~MemoryView()
{

}

void MemoryView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Memory", &m_Open);
	m_MemoryEditor.DrawContents(emu->Memory + KR580VM80A::USER_MEMORY_OFFSET, KR580VM80A::USER_MEMORY_SIZE, KR580VM80A::USER_MEMORY_OFFSET, false, &emu->PC, "PC -> ");
	ImGui::End();
}

bool MemoryView::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
		return false;
	
	file.read((char*)m_Memory, m_MemorySize);

	file.close();
	return true;
}

bool MemoryView::SaveToFile(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
		return false;

	for (int i = 0; i < m_MemorySize; i++)
		file << m_Memory[i];
	
	file.close();
	return true;
}
