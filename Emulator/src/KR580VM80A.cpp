#include "KR580VM80A.h"

KR580VM80A::KR580VM80A()
{
	m_Memory = new uint8_t[MEMORY_SIZE];
	memset(m_Memory, 0, MEMORY_SIZE);
	InitRegisters();
}

KR580VM80A::~KR580VM80A()
{
	delete[] m_Memory;
}

void KR580VM80A::InitRegisters()
{
	A = F = 0;
	B = C = 0;
	D = E = 0;
	H = L = 0;
	PC = 0x0800;
	SP = 0x0BB0;
}

WORD KR580VM80A::Fetch()
{
	return m_Memory[PC++];
}

bool KR580VM80A::LoadProgram(const std::vector<WORD>& program, size_t startAddress /*= 0x0800*/)
{
	for (WORD word : program)
	{
		m_Memory[startAddress++] = word;
	}

	return true;
}
