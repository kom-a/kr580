#pragma once

#include <vector>
#include <inttypes.h>

#define WORD uint8_t
#define DWORD uint16_t
#define MEMORY_SIZE 65536

class KR580VM80A
{
public:
	KR580VM80A();
	~KR580VM80A();

public:
	bool LoadProgram(const std::vector<WORD>& program, size_t startAddress = 0x0800);

private:
	void InitRegisters();
	WORD Fetch();

private:
	uint8_t* m_Memory;

	// Registers
	WORD A, F;	// PSW
	WORD B, C;	// BC
	WORD D, E;	// DE
	WORD H, L;	// HL
	DWORD PC;	// Program counter
	DWORD SP;	// Stack pointer
};
