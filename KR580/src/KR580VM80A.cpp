#include <iostream>
#include <assert.h>
#include <string>
#include <sstream>
#include "KR580VM80A.h"

#include "Log.h"
#include "ISA.h"

#define PAIR(high, low) (((DWORD)high << 8) | low)

namespace KR580
{
	using WORD = KR580VM80A::WORD;
	using DWORD = KR580VM80A::DWORD;
	const uint32_t KR580VM80A::MEMORY_SIZE = 65536;
	const uint32_t KR580VM80A::USER_MEMORY_OFFSET = 0x0800;
	const uint32_t KR580VM80A::USER_MEMORY_SIZE = 0x0B80 - USER_MEMORY_OFFSET;
	const uint32_t KR580VM80A::STACK_OFFSET = 0x0B80;
	const uint32_t KR580VM80A::STACK_SIZE = 0x0BB0 - STACK_OFFSET;
	const uint32_t KR580VM80A::PORTS_SIZE = 256;

	KR580VM80A::KR580VM80A()
	{
		Memory = new uint8_t[MEMORY_SIZE];
		In = new uint8_t[PORTS_SIZE];
		Out = new uint8_t[PORTS_SIZE];

		Init();
	}

	KR580VM80A::~KR580VM80A()
	{
		delete[] Memory;
		delete[] In;
		delete[] Out;
	}

	void KR580VM80A::Init()
	{
		memset(Memory, 0, MEMORY_SIZE);
		memset(In, 0, PORTS_SIZE);
		memset(Out, 0, PORTS_SIZE);

		InitRegisters();
		Running = false;
	}

	void KR580VM80A::InitRegisters()
	{
		PSW = 0;
		BC = 0;
		DE = 0;
		HL = 0;
		SP = STACK_OFFSET + STACK_SIZE; // -1 ???
		PC = USER_MEMORY_OFFSET;
		Flag.D1 = 1;
	}

	void KR580VM80A::UpdateZSP(WORD value)
	{
		int ones = 0;
		int tmp = value;
		for (int i = 0; i < 8; i++)
		{
			if (tmp & 0x01)
				ones++;
			tmp >>= 1;
		}

		Flag.Z = value == 0 ? 1 : 0;
		Flag.S = (value >> 7) & 0x01;
		Flag.P = ones % 2 == 0 ? true : false;
	}

	void KR580VM80A::UpdateAC(WORD old_value, WORD new_value)
	{
		Flag.AC = (old_value & 0xF0) != (new_value & 0xF0);
	}

	WORD KR580VM80A::Fetch()
	{
		return Memory[PC++];
	}

	Opcode KR580VM80A::FetchOpcode()
	{
		Opcode opcode = (Opcode)Fetch();
		return opcode;
	}

	DWORD KR580VM80A::FetchAddress()
	{
		WORD low = Fetch();
		WORD high = Fetch();
		DWORD address = PAIR(high, low);
		return address;
	}

	void KR580VM80A::Add(WORD& reg, WORD value)
	{
		Flag.CY = reg > (WORD)0xFF - value ? 1 : 0;
		WORD old = reg;
		reg += value;
		UpdateAC(old, reg);
	}

	void KR580VM80A::Sub(WORD& reg, WORD value)
	{
		Flag.CY = reg < value ? 1 : 0;
		WORD old = reg;
		reg -= value;
		UpdateAC(old, reg);
	}

	void KR580VM80A::Adc(WORD& reg, WORD value)
	{
		WORD old = reg;
		Add(reg, value);
		bool carry = Flag.CY;
		Add(A, Flag.CY);
		Flag.CY = carry ? carry : Flag.CY;
		UpdateAC(old, reg);
	}

	void KR580VM80A::Sbb(WORD& reg, WORD value)
	{
		WORD old = reg;
		Sub(reg, value);
		bool carry = Flag.CY;
		Sub(A, Flag.CY);
		Flag.CY = carry ? carry : Flag.CY;
		UpdateAC(old, reg);
	}

	void KR580VM80A::Push(DWORD value)
	{
		WORD high = (value >> 8) & 0xFF;
		WORD low = value & 0xFF;

		Memory[SP - 1] = high;
		Memory[SP - 2] = low;
		SP -= 2;
	}

	DWORD KR580VM80A::Pop()
	{
		WORD low = Memory[SP];
		WORD high = Memory[SP + 1];
		SP += 2;

		return PAIR(high, low);
	}

	void KR580VM80A::SetBuiltInFunction(size_t memory_address, void (*proc)(KR580VM80A* emu))
	{
		m_BuiltInFunctions[memory_address] = proc;
	}

	void KR580VM80A::Step()
	{
		Opcode opcode = FetchOpcode();

		switch (opcode)
		{
		case Opcode::ADD_A:
		{
			Add(A, A);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_B:
		{
			Add(A, B);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_C:
		{
			Add(A, C);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_D:
		{
			Add(A, D);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_E:
		{
			Add(A, E);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_H:
		{
			Add(A, H);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_L:
		{
			Add(A, L);
			UpdateZSP(A);
		} break;
		case Opcode::ADD_M:
		{
			WORD data = Memory[HL];
			Add(A, data);
			UpdateZSP(A);
		} break;
		case Opcode::ADI_d8:
		{
			WORD d8 = Fetch();
			Add(A, d8);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_A:
		{
			Adc(A, A);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_B:
		{
			Adc(A, B);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_C:
		{
			Adc(A, C);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_D:
		{
			Adc(A, D);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_E:
		{
			Adc(A, E);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_H:
		{
			Adc(A, H);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_L:
		{
			Adc(A, L);
			UpdateZSP(A);
		} break;
		case Opcode::ADC_M:
		{
			Adc(A, Memory[HL]);
			UpdateZSP(A);
		} break;
		case Opcode::ACI_d8:
		{
			WORD d8 = Fetch();
			Adc(A, d8);
			UpdateZSP(A);
		} break;
		case Opcode::ANA_A:
		{
			A &= A;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_B:
		{
			A &= B;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_C:
		{
			A &= C;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_D:
		{
			A &= D;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_E:
		{
			A &= E;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_H:
		{
			A &= H;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_L:
		{
			A &= L;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANA_M:
		{
			WORD data = Memory[HL];
			A &= data;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ANI_d8:
		{
			WORD d8 = Fetch();
			A &= d8;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::CALL_a16:
		{
			DWORD address = FetchAddress();

			if (m_BuiltInFunctions.find(address) != m_BuiltInFunctions.end())
			{
				void (*proc)(KR580VM80A*) = m_BuiltInFunctions[address];
				proc(this);
			}
			else
			{
				Push(PC);
				PC = address;
			}
		} break;
		case Opcode::CZ_a16:
		{
			if (Flag.Z)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CNZ_a16:
		{
			if (!Flag.Z)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CP_a16:
		{
			if (Flag.P)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CM_a16:
		{
			if (Flag.S)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CC_a16:
		{
			if (Flag.CY)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CNC_a16:
		{
			if (!Flag.CY)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CPE_a16:
		{
			if (Flag.P)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CPO_a16:
		{
			if (!Flag.P)
			{
				DWORD address = FetchAddress();
				Push(PC);
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::CMA:
		{
			A = ~A;
		} break;
		case Opcode::CMC:
		{
			Flag.CY = !Flag.CY;
		} break;
		case Opcode::CMP_A:
		{
			WORD old = A;
			Sub(A, A);
			UpdateZSP(A);
			A += old;
		} break;
		case Opcode::CMP_B:
		{
			Sub(A, B);
			UpdateZSP(A);
			A += B;
		} break;
		case Opcode::CMP_C:
		{
			Sub(A, C);
			UpdateZSP(A);
			A += C;
		} break;
		case Opcode::CMP_D:
		{
			Sub(A, D);
			UpdateZSP(A);
			A += D;
		} break;
		case Opcode::CMP_E:
		{
			Sub(A, E);
			UpdateZSP(A);
			A += E;
		} break;
		case Opcode::CMP_H:
		{
			Sub(A, H);
			UpdateZSP(A);
			A += H;
		} break;
		case Opcode::CMP_L:
		{
			Sub(A, L);
			UpdateZSP(A);
			A += L;
		} break;
		case Opcode::CMP_M:
		{
			WORD data = Memory[HL];
			Sub(A, data);
			UpdateZSP(A);
			A += data;
		} break;
		case Opcode::CPI_d8:
		{
			WORD d8 = Fetch();
			Sub(A, d8);
			UpdateZSP(A);
			A += d8;
		} break;
		case Opcode::DAA:
		{
			// TODO: implement
		} break;
		case Opcode::DAD_B:
		{
			if (HL > (DWORD)0xFFFF - BC)
				Flag.CY = 1;
			HL += BC;
		} break;
		case Opcode::DAD_D:
		{
			if (HL > (DWORD)0xFFFF - DE)
				Flag.CY = 1;
			HL += DE;
		} break;
		case Opcode::DAD_H:
		{
			if (HL > (DWORD)0xFFFF - HL)
				Flag.CY = 1;
			HL += HL;
		} break;
		case Opcode::DAD_SP:
		{
			if (HL > (DWORD)0xFFFF - SP)
				Flag.CY = 1;
			HL += SP;
		} break;
		case Opcode::DCR_A:
		{
			Sub(A, 1);
			UpdateZSP(A);
		} break;
		case Opcode::DCR_B:
		{
			Sub(B, 1);
			UpdateZSP(B);
		} break;
		case Opcode::DCR_C:
		{
			Sub(C, 1);
			UpdateZSP(C);
		} break;
		case Opcode::DCR_D:
		{
			Sub(D, 1);
			UpdateZSP(D);
		} break;
		case Opcode::DCR_E:
		{
			Sub(E, 1);
			UpdateZSP(E);
		} break;
		case Opcode::DCR_H:
		{
			Sub(H, 1);
			UpdateZSP(H);
		} break;
		case Opcode::DCR_L:
		{
			Sub(L, 1);
			UpdateZSP(L);
		} break;
		case Opcode::DCR_M:
		{
			Sub(Memory[HL], 1);
			UpdateZSP(Memory[HL]);
		} break;
		case Opcode::DCX_B:
		{
			BC--;
		} break;
		case Opcode::DCX_D:
		{
			DE--;
		} break;
		case Opcode::DCX_H:
		{
			HL--;
		} break;
		case Opcode::DCX_SP:
		{
			SP--;
		} break;
		case Opcode::DI:
		{
			// TODO: implement
			assert(false, "Not implemented");
		} break;
		case Opcode::EI:
		{
			// TODO: implement
			assert(false, "Not implemented");
		} break;
		case Opcode::HLT:
		{
			// TODO: implement
			assert(false, "Not implemented");
		} break;
		case Opcode::IN_pp:
		{
			uint8_t port = Fetch();
			A = In[port];
		} break;
		case Opcode::INR_A:
		{
			Add(A, 1);
			UpdateZSP(A);
		} break;
		case Opcode::INR_B:
		{
			Add(B, 1);
			UpdateZSP(B);
		} break;
		case Opcode::INR_C:
		{
			Add(C, 1);
			UpdateZSP(C);
		} break;
		case Opcode::INR_D:
		{
			Add(D, 1);
			UpdateZSP(D);
		} break;
		case Opcode::INR_E:
		{
			Add(E, 1);
			UpdateZSP(E);
		} break;
		case Opcode::INR_H:
		{
			Add(H, 1);
			UpdateZSP(H);
		} break;
		case Opcode::INR_L:
		{
			Add(L, 1);
			UpdateZSP(L);
		} break;
		case Opcode::INR_M:
		{
			Add(Memory[HL], 1);
			UpdateZSP(Memory[HL]);
		} break;
		case Opcode::INX_B:
		{
			BC++;
		} break;
		case Opcode::INX_D:
		{
			DE++;
		} break;
		case Opcode::INX_H:
		{
			HL++;
		} break;
		case Opcode::INX_SP:
		{
			SP++;
		} break;
		case Opcode::JMP_a16:
		{
			DWORD address = FetchAddress();
			PC = address;
		} break;
		case Opcode::JZ_a16:
		{
			if (Flag.Z)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JNZ_a16:
		{
			if (!Flag.Z)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JP_a16:
		{
			if (!Flag.S)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JM_a16:
		{
			if (Flag.S)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JC_a16:
		{
			if (Flag.CY)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JNC_a16:
		{
			if (!Flag.CY)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JPE_a16:
		{
			if (Flag.P)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::JPO_a16:
		{
			if (!Flag.P)
			{
				DWORD address = FetchAddress();
				PC = address;
			}
			else
				PC += 2;
		} break;
		case Opcode::LDA_a16:
		{
			DWORD address = FetchAddress();
			A = Memory[address];
		} break;
		case Opcode::LDAX_B:
		{
			A = Memory[BC];
		} break;
		case Opcode::LDAX_D:
		{
			A = Memory[DE];
		} break;
		case Opcode::LHLD_a16:
		{
			DWORD address = FetchAddress();
			L = Memory[address + 0];
			H = Memory[address + 1];
		} break;
		case Opcode::LXI_B_d16:
		{
			DWORD data = FetchAddress();
			BC = data;
		} break;
		case Opcode::LXI_D_d16:
		{
			DWORD data = FetchAddress();
			DE = data;
		} break;
		case Opcode::LXI_H_d16:
		{
			DWORD data = FetchAddress();
			HL = data;
		} break;
		case Opcode::LXI_SP_d16:
		{
			DWORD data = FetchAddress();
			SP = data;
		} break;
		case Opcode::MOV_A_A:
		{
			A = A;
		} break;
		case Opcode::MOV_A_B:
		{
			A = B;
		} break;
		case Opcode::MOV_A_C:
		{
			A = C;
		} break;
		case Opcode::MOV_A_D:
		{
			A = D;
		} break;
		case Opcode::MOV_A_E:
		{
			A = E;
		} break;
		case Opcode::MOV_A_H:
		{
			A = H;
		} break;
		case Opcode::MOV_A_L:
		{
			A = L;
		} break;
		case Opcode::MOV_A_M:
		{
			A = Memory[HL];
		} break;
		case Opcode::MOV_B_A:
		{
			B = A;
		} break;
		case Opcode::MOV_B_B:
		{
			B = B;
		} break;
		case Opcode::MOV_B_C:
		{
			B = C;
		} break;
		case Opcode::MOV_B_D:
		{
			B = D;
		} break;
		case Opcode::MOV_B_E:
		{
			B = E;
		} break;
		case Opcode::MOV_B_H:
		{
			B = H;
		} break;
		case Opcode::MOV_B_L:
		{
			B = L;
		} break;
		case Opcode::MOV_B_M:
		{
			B = Memory[HL];
		} break;
		case Opcode::MOV_C_A:
		{
			C = A;
		} break;
		case Opcode::MOV_C_B:
		{
			C = B;
		} break;
		case Opcode::MOV_C_C:
		{
			C = C;
		} break;
		case Opcode::MOV_C_D:
		{
			C = D;
		} break;
		case Opcode::MOV_C_E:
		{
			C = E;
		} break;
		case Opcode::MOV_C_H:
		{
			C = H;
		} break;
		case Opcode::MOV_C_L:
		{
			C = L;
		} break;
		case Opcode::MOV_C_M:
		{
			C = Memory[HL];
		} break;
		case Opcode::MOV_D_A:
		{
			D = A;
		} break;
		case Opcode::MOV_D_B:
		{
			D = B;
		} break;
		case Opcode::MOV_D_C:
		{
			D = C;
		} break;
		case Opcode::MOV_D_D:
		{
			D = D;
		} break;
		case Opcode::MOV_D_E:
		{
			D = E;
		} break;
		case Opcode::MOV_D_H:
		{
			D = H;
		} break;
		case Opcode::MOV_D_L:
		{
			D = L;
		} break;
		case Opcode::MOV_D_M:
		{
			D = Memory[HL];
		} break;
		case Opcode::MOV_E_A:
		{
			E = A;
		} break;
		case Opcode::MOV_E_B:
		{
			E = B;
		} break;
		case Opcode::MOV_E_C:
		{
			E = C;
		} break;
		case Opcode::MOV_E_D:
		{
			E = D;
		} break;
		case Opcode::MOV_E_E:
		{
			E = E;
		} break;
		case Opcode::MOV_E_H:
		{
			E = H;
		} break;
		case Opcode::MOV_E_L:
		{
			E = L;
		} break;
		case Opcode::MOV_E_M:
		{
			E = Memory[HL];
		} break;
		case Opcode::MOV_H_A:
		{
			H = A;
		} break;
		case Opcode::MOV_H_B:
		{
			H = B;
		} break;
		case Opcode::MOV_H_C:
		{
			H = C;
		} break;
		case Opcode::MOV_H_D:
		{
			H = D;
		} break;
		case Opcode::MOV_H_E:
		{
			H = E;
		} break;
		case Opcode::MOV_H_H:
		{
			H = H;
		} break;
		case Opcode::MOV_H_L:
		{
			H = L;
		} break;
		case Opcode::MOV_H_M:
		{
			H = Memory[HL];
		} break;
		case Opcode::MOV_L_A:
		{
			L = A;
		} break;
		case Opcode::MOV_L_B:
		{
			L = B;
		} break;
		case Opcode::MOV_L_C:
		{
			L = C;
		} break;
		case Opcode::MOV_L_D:
		{
			L = D;
		} break;
		case Opcode::MOV_L_E:
		{
			L = E;
		} break;
		case Opcode::MOV_L_H:
		{
			L = H;
		} break;
		case Opcode::MOV_L_L:
		{
			L = L;
		} break;
		case Opcode::MOV_L_M:
		{
			L = Memory[HL];
		} break;
		case Opcode::MOV_M_A:
		{
			Memory[HL] = A;
		} break;
		case Opcode::MOV_M_B:
		{
			Memory[HL] = B;
		} break;
		case Opcode::MOV_M_C:
		{
			Memory[HL] = C;
		} break;
		case Opcode::MOV_M_D:
		{
			Memory[HL] = D;
		} break;
		case Opcode::MOV_M_E:
		{
			Memory[HL] = E;
		} break;
		case Opcode::MOV_M_H:
		{
			Memory[HL] = H;
		} break;
		case Opcode::MOV_M_L:
		{
			Memory[HL] = L;
		} break;
		case Opcode::MVI_A_d8:
		{
			WORD d8 = Fetch();
			A = d8;
		} break;
		case Opcode::MVI_B_d8:
		{
			WORD d8 = Fetch();
			B = d8;
		} break;
		case Opcode::MVI_C_d8:
		{
			WORD d8 = Fetch();
			C = d8;
		} break;
		case Opcode::MVI_D_d8:
		{
			WORD d8 = Fetch();
			D = d8;
		} break;
		case Opcode::MVI_E_d8:
		{
			WORD d8 = Fetch();
			E = d8;
		} break;
		case Opcode::MVI_H_d8:
		{
			WORD d8 = Fetch();
			H = d8;
		} break;
		case Opcode::MVI_L_d8:
		{
			WORD d8 = Fetch();
			L = d8;
		} break;
		case Opcode::MVI_M_d8:
		{
			WORD d8 = Fetch();
			Memory[HL] = d8;
		} break;
		case Opcode::NOP:
		{
			// No operation
		} break;
		case Opcode::ORA_A:
		{
			A |= A;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_B:
		{
			A |= B;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_C:
		{
			A |= C;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_D:
		{
			A |= D;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_E:
		{
			A |= E;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_H:
		{
			A |= H;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_L:
		{
			A |= L;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORA_M:
		{
			A |= Memory[HL];
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::ORI_d8:
		{
			WORD d8 = Fetch();
			A |= d8;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::OUT_pp:
		{
			uint8_t port = Fetch();
			Out[port] = A;
		} break;
		case Opcode::PCHL:
		{
			PC = HL;
		} break;
		case Opcode::POP_B:
		{
			BC = Pop();
		} break;
		case Opcode::POP_D:
		{
			DE = Pop();
		} break;
		case Opcode::POP_H:
		{
			HL = Pop();
		} break;
		case Opcode::POP_PSW:
		{
			PSW = Pop();
		} break;
		case Opcode::PUSH_B:
		{
			Push(BC);
		} break;
		case Opcode::PUSH_D:
		{
			Push(DE);
		} break;
		case Opcode::PUSH_H:
		{
			Push(HL);
		} break;
		case Opcode::PUSH_PSW:
		{
			Push(PSW);
		} break;
		case Opcode::RAL:
		{
			bool mostSignificantBit = (A >> 7) & 0x01;
			A <<= 1;
			A |= Flag.CY ? 0x01 : 0x00;
			Flag.CY = mostSignificantBit;
		} break;
		case Opcode::RAR:
		{
			bool leastSignificantBit = A & 0x01;
			A >>= 1;
			A &= 0x7F;
			A |= (WORD)Flag.CY << 7;
			Flag.CY = leastSignificantBit;
		} break;
		case Opcode::RLC:
		{
			bool mostSignificantBit = (A >> 7) & 0x01;
			A <<= 1;
			A |= (WORD)mostSignificantBit;
			Flag.CY = mostSignificantBit;
		} break;
		case Opcode::RRC:
		{
			bool leastSignificantBit = A & 0x01;
			A >>= 1;
			A &= 0x7F;
			A |= (WORD)leastSignificantBit << 7;
			Flag.CY = leastSignificantBit;
		} break;
		case Opcode::RIM:
		{
			assert(false, "Not implemented");
		} break;
		case Opcode::RET:
		{
			PC = Pop();
		} break;
		case Opcode::RZ:
		{
			if (Flag.Z)
				PC = Pop();
		} break;
		case Opcode::RNZ:
		{
			if (!Flag.Z)
				PC = Pop();
		} break;
		case Opcode::RP:
		{
			if (!Flag.S)
				PC = Pop();
		} break;
		case Opcode::RM:
		{
			if (Flag.S)
				PC = Pop();
		} break;
		case Opcode::RC:
		{
			if (Flag.CY)
				PC = Pop();
		} break;
		case Opcode::RNC:
		{
			if (!Flag.CY)
				PC = Pop();
		} break;
		case Opcode::RPE:
		{
			if (Flag.P)
				PC = Pop();
		} break;
		case Opcode::RPO:
		{
			if (!Flag.P)
				PC = Pop();
		} break;
		case Opcode::RST_0:
		{
			Push(PC);
			PC = 0 * 0x08;
		} break;
		case Opcode::RST_1:
		{
			// Push(PC);
			// PC = 1 * 0x08;
			Running = false;
		} break;
		case Opcode::RST_2:
		{
			Push(PC);
			PC = 2 * 0x08;
		} break;
		case Opcode::RST_3:
		{
			Push(PC);
			PC = 3 * 0x08;
		} break;
		case Opcode::RST_4:
		{
			Push(PC);
			PC = 4 * 0x08;
		} break;
		case Opcode::RST_5:
		{
			Push(PC);
			PC = 5 * 0x08;
		} break;
		case Opcode::RST_6:
		{
			Push(PC);
			PC = 6 * 0x08;
		} break;
		case Opcode::RST_7:
		{
			Push(PC);
			PC = 7 * 0x08;
		} break;
		case Opcode::SIM:
		{
			assert(false, "Not implemented");
		} break;
		case Opcode::SPHL:
		{
			SP = HL;
		} break;
		case Opcode::SHLD_a16:
		{
			DWORD address = FetchAddress();
			Memory[address + 0] = L;
			Memory[address + 1] = H;
		} break;
		case Opcode::STA_a16:
		{
			DWORD address = FetchAddress();
			Memory[address] = A;
		} break;
		case Opcode::STAX_B:
		{
			Memory[BC] = A;
		} break;
		case Opcode::STAX_D:
		{
			Memory[DE] = A;
		} break;
		case Opcode::STC:
		{
			Flag.CY = true;
		} break;
		case Opcode::SUB_A:
		{
			Sub(A, A);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_B:
		{
			Sub(A, B);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_C:
		{
			Sub(A, C);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_D:
		{
			Sub(A, D);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_E:
		{
			Sub(A, E);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_H:
		{
			Sub(A, H);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_L:
		{
			Sub(A, L);
			UpdateZSP(A);
		} break;
		case Opcode::SUB_M:
		{
			Sub(A, Memory[HL]);
			UpdateZSP(A);
		} break;
		case Opcode::SUI_d8:
		{
			WORD d8 = Fetch();
			Sub(A, d8);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_A:
		{
			Sbb(A, A);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_B:
		{
			Sbb(A, B);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_C:
		{
			Sbb(A, C);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_D:
		{
			Sbb(A, D);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_E:
		{
			Sbb(A, E);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_H:
		{
			Sbb(A, H);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_L:
		{
			Sbb(A, L);
			UpdateZSP(A);
		} break;
		case Opcode::SBB_M:
		{
			Sbb(A, Memory[HL]);
			UpdateZSP(A);
		} break;
		case Opcode::SBI_d8:
		{
			WORD d8 = Fetch();
			Sbb(A, d8);
			UpdateZSP(A);
		} break;
		case Opcode::XCHG:
		{
			DWORD tmp = HL;
			HL = DE;
			DE = tmp;
		} break;
		case Opcode::XTHL:
		{
			DWORD tmp = HL;
			HL = Pop();
			Push(tmp);
		} break;
		case Opcode::XRA_A:
		{
			A ^= A;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_B:
		{
			A ^= B;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_C:
		{
			A ^= C;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_D:
		{
			A ^= D;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_E:
		{
			A ^= E;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_H:
		{
			A ^= H;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_L:
		{
			A ^= L;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRA_M:
		{
			A ^= Memory[HL];
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		case Opcode::XRI_d8:
		{
			WORD d8 = Fetch();
			A ^= d8;
			UpdateZSP(A);
			Flag.AC = 0;
			Flag.CY = 0;
		} break;
		default:
		{
			PC--;
			Running = false;

			std::stringstream ss;
			ss << "Undefined opcode \'0x" << std::hex << (int)opcode << std::dec << "\'";
			LOG_ERROR("{0}", ss.str());
		}
		}
	}

	bool KR580VM80A::LoadProgram(const std::vector<WORD>& program, size_t startAddress /*= 0x0800*/)
	{
		for (WORD word : program)
		{
			Memory[startAddress++] = word;
		}

		return true;
	}

}