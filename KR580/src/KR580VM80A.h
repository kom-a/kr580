#pragma once

#include <vector>
#include <inttypes.h>
#include <unordered_map>

namespace KR580
{
	enum class Opcode : uint8_t;

	class KR580VM80A
	{
	public:
		KR580VM80A();
		~KR580VM80A();

	public:
		typedef uint8_t WORD;
		typedef uint16_t DWORD;
		static const uint32_t MEMORY_SIZE;
		static const uint32_t USER_MEMORY_OFFSET;
		static const uint32_t USER_MEMORY_SIZE;
		static const uint32_t STACK_OFFSET;
		static const uint32_t STACK_SIZE;
		static const uint32_t PORTS_SIZE;

	public:
		void Init();
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

	public:
		void SetBuiltInFunction(size_t memory_address, void (*proc)(KR580VM80A* emu));
	public:
		uint8_t* Memory;
		uint8_t* In, * Out; // Ports

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

	private:
		std::unordered_map<size_t, void (*)(KR580VM80A*)> m_BuiltInFunctions; // Key - memory address, Value - procedure function pointer that handles built in call
	};

}