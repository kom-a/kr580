#pragma once

#include <vector>
#include <inttypes.h>

#define WORD uint8_t
#define DWORD uint16_t
#define MEMORY_SIZE 65536
#define USER_MEMORY_OFFSET 0x0800
#define USER_MEMORY_SIZE (0x0B80 - USER_MEMORY_OFFSET)
#define STACK_OFFSET 0x0B80
#define STACK_SIZE (0x0BB0 - STACK_OFFSET)
#define PORTS_SIZE 256

enum Opcode : uint8_t;

class KR580VM80A
{
public:
	KR580VM80A();
	~KR580VM80A();

public:
	bool LoadProgram(const std::vector<WORD>& program, size_t startAddress = 0x0800);
	void Step();

private:
	void InitRegisters();
	void UpdateZSP(WORD value);
	void UpdateAC(WORD old_value, WORD new_value);
	WORD Fetch();
	Opcode FetchOpcode();
	DWORD FetchAddress();
	void Add(WORD& reg, WORD value);
	void Sub(WORD& reg, WORD value);
	void Adc(WORD& reg, WORD value);
	void Sbb(WORD& reg, WORD value);
	void Push(DWORD value);
	DWORD Pop();
	bool IsBuildInAddress(DWORD address);

public:
	uint8_t* Memory;
	uint8_t *In, *Out; // Ports
	
	bool Running;

	// A, F - PSW
	union
	{
		struct
		{
			union
			{
				struct
				{
					bool CY : 1; // Carry flag
					bool D1 : 1; // Unused bit
					bool P : 1; // Parity flag
					bool D3 : 1; // Unused bit
					bool AC : 1; // Auxiliary carry flag
					bool D5 : 1; // Unused bit
					bool Z : 1; // Zero
					bool S : 1; // Sign
				} Flag;
				WORD F;
			};
			WORD A;
		};
		DWORD PSW;
	};

	// B, C - BC
	union
	{
		struct
		{
			WORD C;
			WORD B;
		};
		DWORD BC;
	};

	// D, E - DE
	union
	{
		struct
		{
			WORD E;
			WORD D;
		};
		DWORD DE;
	};

	// H, L - HL
	union 
	{
		struct
		{
			WORD L;
			WORD H;
		};
		DWORD HL;
	};

	DWORD SP; // Stack pointer
	DWORD PC; // Program counter
};
