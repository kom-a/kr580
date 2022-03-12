#include <iostream>
#include <assert.h>
#include "KR580VM80A.h"

#include "ISA.h"

#define PAIR(high, low) (((DWORD)high << 8) | low)

KR580VM80A::KR580VM80A()
{
	Memory = new uint8_t[MEMORY_SIZE];
	In = new uint8_t[PORTS_SIZE];
	Out = new uint8_t[PORTS_SIZE];

	memset(Memory,	0, MEMORY_SIZE);
	memset(In,		0, PORTS_SIZE);
	memset(Out,		0, PORTS_SIZE);

	InitRegisters();
	Running = false;
}

KR580VM80A::~KR580VM80A()
{
	delete[] Memory;
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
	reg += value;
}

void KR580VM80A::Sub(WORD& reg, WORD value)
{
	Flag.CY = reg < value ? 1 : 0;
	reg -= value;
}

void KR580VM80A::Adc(WORD& reg, WORD value)
{
	Add(reg, value);
	bool carry = Flag.CY;
	Add(A, Flag.CY);
	Flag.CY = carry ? carry : Flag.CY;
}

void KR580VM80A::Sbb(WORD& reg, WORD value)
{
	Sub(reg, value);
	bool carry = Flag.CY;
	Sub(A, Flag.CY);
	Flag.CY = carry ? carry : Flag.CY;
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

void KR580VM80A::Step()
{
	Opcode opcode = FetchOpcode();

	switch (opcode)
	{
	case ADD_A:
	{
		Add(A, A);
		UpdateZSP(A);
	} break;
	case ADD_B:
	{
		Add(A, B);
		UpdateZSP(A);
	} break;
	case ADD_C:
	{
		Add(A, C);
		UpdateZSP(A);
	} break;
	case ADD_D:
	{
		Add(A, D);
		UpdateZSP(A);
	} break;
	case ADD_E:
	{
		Add(A, E);
		UpdateZSP(A);
	} break;
	case ADD_H:
	{
		Add(A, H);
		UpdateZSP(A);
	} break;
	case ADD_L:
	{
		Add(A, L);
		UpdateZSP(A);
	} break;
	case ADD_M:
	{
		WORD data = Memory[HL];
		Add(A, data);
		UpdateZSP(A);
	} break;
	case ADI_d8:
	{
		WORD d8 = Fetch();
		Add(A, d8);
		UpdateZSP(A);
	} break;
	case ADC_A:
	{
		Adc(A, A);
		UpdateZSP(A);
	} break;
	case ADC_B:
	{
		Adc(A, B);
		UpdateZSP(A);
	} break;
	case ADC_C:
	{
		Adc(A, C);
		UpdateZSP(A);
	} break;
	case ADC_D:
	{
		Adc(A, D);
		UpdateZSP(A);
	} break;
	case ADC_E:
	{
		Adc(A, E);
		UpdateZSP(A);
	} break;
	case ADC_H:
	{
		Adc(A, H);
		UpdateZSP(A);
	} break;
	case ADC_L:
	{
		Adc(A, L);
		UpdateZSP(A);
	} break;
	case ADC_M:
	{
		Adc(A, Memory[HL]);
		UpdateZSP(A);
	} break;
	case ACI_d8:
	{
		WORD d8 = Fetch();
		Adc(A, d8);
		UpdateZSP(A);
	} break;
	case ANA_A:
	{
		A &= A;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_B:
	{
		A &= B;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_C:
	{
		A &= C;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_D:
	{
		A &= D;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_E:
	{
		A &= E;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_H:
	{
		A &= H;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_L:
	{
		A &= L;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANA_M:
	{
		WORD data = Memory[HL];
		A &= data;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ANI_d8:
	{
		WORD d8 = Fetch();
		A &= d8;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case CALL_a16:
	{
		DWORD address = FetchAddress();
		Push(PC);
		PC = address;
	} break;
	case CZ_a16:
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
	case CNZ_a16:
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
	case CP_a16:
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
	case CM_a16:
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
	case CC_a16:
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
	case CNC_a16:
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
	case CPE_a16:
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
	case CPO_a16:
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
	case CMA:
	{
		A = ~A;
	} break;
	case CMC:
	{
		Flag.CY = !Flag.CY;
	} break;
	case CMP_A:
	{
		UpdateZSP(0);
	} break;
	case CMP_B:
	{
		Sub(A, B);
		UpdateZSP(A);
		A += B;
	} break;
	case CMP_C:
	{
		Sub(A, C);
		UpdateZSP(A);
		A += C;
	} break;
	case CMP_D:
	{
		Sub(A, D);
		UpdateZSP(A);
		A += D;
	} break;
	case CMP_E:
	{
		Sub(A, E);
		UpdateZSP(A);
		A += E;
	} break;
	case CMP_H:
	{
		Sub(A, H);
		UpdateZSP(A);
		A += H;
	} break;
	case CMP_L:
	{
		Sub(A, L);
		UpdateZSP(A);
		A += L;
	} break;
	case CMP_M:
	{
		WORD data = Memory[HL];
		Sub(A, data);
		UpdateZSP(A);
		A += data;
	} break;
	case CPI_d8:
	{
		WORD d8 = Fetch();
		Sub(A, d8);
		UpdateZSP(A);
		A += d8;
	} break;
	case DAA:
	{
		// TODO: implement
	} break;
	case DAD_B:
	{
		if (HL > (DWORD)0xFFFF - BC)
			Flag.CY = 1;
		HL += BC;
	} break;
	case DAD_D:
	{
		if (HL > (DWORD)0xFFFF - DE)
			Flag.CY = 1;
		HL += DE;
	} break;
	case DAD_H:
	{
		if (HL > (DWORD)0xFFFF - HL)
			Flag.CY = 1;
		HL += HL;
	} break;
	case DAD_SP:
	{
		if (HL > (DWORD)0xFFFF - SP)
			Flag.CY = 1;
		HL += SP;
	} break;
	case DCR_A:
	{
		Sub(A, 1);
		UpdateZSP(A);
	} break;
	case DCR_B:
	{
		Sub(B, 1);
		UpdateZSP(B);
	} break;
	case DCR_C:
	{
		Sub(C, 1);
		UpdateZSP(C);
	} break;
	case DCR_D:
	{
		Sub(D, 1);
		UpdateZSP(D);
	} break;
	case DCR_E:
	{
		Sub(E, 1);
		UpdateZSP(E);
	} break;
	case DCR_H:
	{
		Sub(H, 1);
		UpdateZSP(H);
	} break;
	case DCR_L:
	{
		Sub(L, 1);
		UpdateZSP(L);
	} break;
	case DCR_M:
	{
		Sub(Memory[HL], 1);
		UpdateZSP(Memory[HL]);
	} break;
	case DCX_B:
	{
		BC--;
	} break;
	case DCX_D:
	{
		DE--;
	} break;
	case DCX_H:
	{
		HL--;
	} break;
	case DCX_SP:
	{
		SP--;
	} break;
	case DI:
	{
		// TODO: implement
		assert(false, "Not implemented");
	} break;
	case EI:
	{
		// TODO: implement
		assert(false, "Not implemented");
	} break;
	case HLT:
	{
		// TODO: implement
		assert(false, "Not implemented");
	} break;
	case IN_pp:
	{
		uint8_t port = Fetch();
		A = In[port];
	} break;
	case INR_A:
	{
		Add(A, 1);
		UpdateZSP(A);
	} break;
	case INR_B:
	{
		Add(B, 1);
		UpdateZSP(B);
	} break;
	case INR_C:
	{
		Add(C, 1);
		UpdateZSP(C);
	} break;
	case INR_D:
	{
		Add(D, 1);
		UpdateZSP(D);
	} break;
	case INR_E:
	{
		Add(E, 1);
		UpdateZSP(E);
	} break;
	case INR_H:
	{
		Add(H, 1);
		UpdateZSP(H);
	} break;
	case INR_L:
	{
		Add(L, 1);
		UpdateZSP(L);
	} break;
	case INR_M:
	{
		Add(Memory[HL], 1);
		UpdateZSP(Memory[HL]);
	} break;
	case INX_B:
	{
		BC++;
	} break;
	case INX_D:
	{
		DE++;
	} break;
	case INX_H:
	{
		HL++;
	} break;
	case INX_SP:
	{
		SP++;
	} break;
	case JMP_a16:
	{
		DWORD address = FetchAddress();
		PC = address;
	} break;
	case JZ_a16:
	{
		if (Flag.Z)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JNZ_a16:
	{
		if (!Flag.Z)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JP_a16:
	{
		if (!Flag.S)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JM_a16:
	{
		if (Flag.S)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JC_a16:
	{
		if (Flag.CY)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JNC_a16:
	{
		if (!Flag.CY)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JPE_a16:
	{
		if (Flag.P)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case JPO_a16:
	{
		if (!Flag.P)
		{
			DWORD address = FetchAddress();
			PC = address;
		}
		else
			PC += 2;
	} break;
	case LDA_a16:
	{
		DWORD address = FetchAddress();
		A = Memory[address];
	} break;
	case LDAX_B:
	{
		A = Memory[BC];
	} break;
	case LDAX_D:
	{
		A = Memory[DE];
	} break;
	case LHLD_a16:
	{
		DWORD address = FetchAddress();
		L = Memory[address + 0];
		H = Memory[address + 1];
	} break;
	case LXI_B_d16:
	{
		DWORD data = FetchAddress();
		BC = data;
	} break;
	case LXI_D_d16:
	{
		DWORD data = FetchAddress();
		DE = data;
	} break;
	case LXI_H_d16:
	{
		DWORD data = FetchAddress();
		HL = data;
	} break;
	case LXI_SP_d16:
	{
		DWORD data = FetchAddress();
		SP = data;
	} break;
	case MOV_A_A:
	{
		A = A;
	} break;
	case MOV_A_B:
	{
		A = B;
	} break;
	case MOV_A_C:
	{
		A = C;
	} break;
	case MOV_A_D:
	{
		A = D;
	} break;
	case MOV_A_E:
	{
		A = E;
	} break;
	case MOV_A_H:
	{
		A = H;
	} break;
	case MOV_A_L:
	{
		A = L;
	} break;
	case MOV_A_M:
	{
		A = Memory[HL];
	} break;
	case MOV_B_A:
	{
		B = A;
	} break;
	case MOV_B_B:
	{
		B = B;
	} break;
	case MOV_B_C:
	{
		B = C;
	} break;
	case MOV_B_D:
	{
		B = D;
	} break;
	case MOV_B_E:
	{
		B = E;
	} break;
	case MOV_B_H:
	{
		B = H;
	} break;
	case MOV_B_L:
	{
		B = L;
	} break;
	case MOV_B_M:
	{
		B = Memory[HL];
	} break;
	case MOV_C_A:
	{
		C = A;
	} break;
	case MOV_C_B:
	{
		C = B;
	} break;
	case MOV_C_C:
	{
		C = C;
	} break;
	case MOV_C_D:
	{
		C = D;
	} break;
	case MOV_C_E:
	{
		C = E;
	} break;
	case MOV_C_H:
	{
		C = H;
	} break;
	case MOV_C_L:
	{
		C = L;
	} break;
	case MOV_C_M:
	{
		C = Memory[HL];
	} break;
	case MOV_D_A:
	{
		D = A;
	} break;
	case MOV_D_B:
	{
		D = B;
	} break;
	case MOV_D_C:
	{
		D = C;
	} break;
	case MOV_D_D:
	{
		D = D;
	} break;
	case MOV_D_E:
	{
		D = E;
	} break;
	case MOV_D_H:
	{
		D = H;
	} break;
	case MOV_D_L:
	{
		D = L;
	} break;
	case MOV_D_M:
	{
		D = Memory[HL];
	} break;
	case MOV_E_A:
	{
		E = A;
	} break;
	case MOV_E_B:
	{
		E = B;
	} break;
	case MOV_E_C:
	{
		E = C;
	} break;
	case MOV_E_D:
	{
		E = D;
	} break;
	case MOV_E_E:
	{
		E = E;
	} break;
	case MOV_E_H:
	{
		E = H;
	} break;
	case MOV_E_L:
	{
		E = L;
	} break;
	case MOV_E_M:
	{
		E = Memory[HL];
	} break;
	case MOV_H_A:
	{
		H = A;
	} break;
	case MOV_H_B:
	{
		H = B;
	} break;
	case MOV_H_C:
	{
		H = C;
	} break;
	case MOV_H_D:
	{
		H = D;
	} break;
	case MOV_H_E:
	{
		H = E;
	} break;
	case MOV_H_H:
	{
		H = H;
	} break;
	case MOV_H_L:
	{
		H = L;
	} break;
	case MOV_H_M:
	{
		H = Memory[HL];
	} break;
	case MOV_L_A:
	{
		L = A;
	} break;
	case MOV_L_B:
	{
		L = B;
	} break;
	case MOV_L_C:
	{
		L = C;
	} break;
	case MOV_L_D:
	{
		L = D;
	} break;
	case MOV_L_E:
	{
		L = E;
	} break;
	case MOV_L_H:
	{
		L = H;
	} break;
	case MOV_L_L:
	{
		L = L;
	} break;
	case MOV_L_M:
	{
		L = Memory[HL];
	} break;
	case MOV_M_A:
	{
		Memory[HL] = A;
	} break;
	case MOV_M_B:
	{
		Memory[HL] = B;
	} break;
	case MOV_M_C:
	{
		Memory[HL] = C;
	} break;
	case MOV_M_D:
	{
		Memory[HL] = D;
	} break;
	case MOV_M_E:
	{
		Memory[HL] = E;
	} break;
	case MOV_M_H:
	{
		Memory[HL] = H;
	} break;
	case MOV_M_L:
	{
		Memory[HL] = L;
	} break;
	case MVI_A_d8:
	{
		WORD d8 = Fetch();
		A = d8;
	} break;
	case MVI_B_d8:
	{
		WORD d8 = Fetch();
		B = d8;
	} break;
	case MVI_C_d8:
	{
		WORD d8 = Fetch();
		C = d8;
	} break;
	case MVI_D_d8:
	{
		WORD d8 = Fetch();
		D = d8;
	} break;
	case MVI_E_d8:
	{
		WORD d8 = Fetch();
		E = d8;
	} break;
	case MVI_H_d8:
	{
		WORD d8 = Fetch();
		H = d8;
	} break;
	case MVI_L_d8:
	{
		WORD d8 = Fetch();
		L = d8;
	} break;
	case MVI_M_d8:
	{
		WORD d8 = Fetch();
		Memory[HL] = d8;
	} break;
	case NOP:
	{
		// No operation
	} break;
	case ORA_A:
	{
		A |= A;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_B:
	{
		A |= B;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_C:
	{
		A |= C;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_D:
	{
		A |= D;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_E:
	{
		A |= E;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_H:
	{
		A |= H;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_L:
	{
		A |= L;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORA_M:
	{
		A |= Memory[HL];
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case ORI_d8:
	{
		WORD d8 = Fetch();
		A |= d8;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case OUT_pp:
	{
		uint8_t port = Fetch();
		Out[port] = A;
	} break;
	case PCHL:
	{
		PC = HL;
	} break;
	case POP_B:
	{
		BC = Pop();
	} break;
	case POP_D:
	{
		DE = Pop();
	} break;
	case POP_H:
	{
		HL = Pop();
	} break;
	case POP_PSW:
	{
		PSW = Pop();
	} break;
	case PUSH_B:
	{
		Push(BC);
	} break;
	case PUSH_D:
	{
		Push(DE);
	} break;
	case PUSH_H:
	{
		Push(HL);
	} break;
	case PUSH_PSW:
	{
		Push(PSW);
	} break;
	case RAL:
	{
		bool mostSignificantBit = (A >> 7) & 0x01;
		A <<= 1;
		A |= Flag.CY ? 0x01 : 0x00;
		Flag.CY = mostSignificantBit;
	} break;
	case RAR:
	{
		bool leastSignificantBit = A & 0x01;
		A >>= 1;
		A &= 0x7F;
		A |= (WORD)Flag.CY << 7;
		Flag.CY = leastSignificantBit;
	} break;
	case RLC:
	{
		bool mostSignificantBit = (A >> 7) & 0x01;
		A <<= 1;
		A |= (WORD)mostSignificantBit;
		Flag.CY = mostSignificantBit;
	} break;
	case RRC:
	{
		bool leastSignificantBit = A & 0x01;
		A >>= 1;
		A &= 0x7F;
		A |= (WORD)leastSignificantBit << 7;
		Flag.CY = leastSignificantBit;
	} break;
	case RIM:
	{
		assert(false, "Not implemented");
	} break;
	case RET:
	{
		PC = Pop();
	} break;
	case RZ:
	{
		if (Flag.Z)
			PC = Pop();
	} break;
	case RNZ:
	{
		if (!Flag.Z)
			PC = Pop();
	} break;
	case RP:
	{
		if (!Flag.S)
			PC = Pop();
	} break;
	case RM:
	{
		if (Flag.S)
			PC = Pop();
	} break;
	case RC:
	{
		if (Flag.CY)
			PC = Pop();
	} break;
	case RNC:
	{
		if (!Flag.CY)
			PC = Pop();
	} break;
	case RPE:
	{
		if (Flag.P)
			PC = Pop();
	} break;
	case RPO:
	{
		if (!Flag.P)
			PC = Pop();
	} break;
	case RST_0:
	{
		Push(PC);
		PC = 0 * 0x08;
	} break;
	case RST_1:
	{
		// Push(PC);
		// PC = 1 * 0x08;
		Running = false;
	} break;
	case RST_2:
	{
		Push(PC);
		PC = 2 * 0x08;
	} break;
	case RST_3:
	{
		Push(PC);
		PC = 3 * 0x08;
	} break;
	case RST_4:
	{
		Push(PC);
		PC = 4 * 0x08;
	} break;
	case RST_5:
	{
		Push(PC);
		PC = 5 * 0x08;
	} break;
	case RST_6:
	{
		Push(PC);
		PC = 6 * 0x08;
	} break;
	case RST_7:
	{
		Push(PC);
		PC = 7 * 0x08;
	} break;
	case SIM:
	{
		assert(false, "Not implemented");
	} break;
	case SPHL:
	{
		SP = HL;
	} break;
	case SHLD_a16:
	{
		DWORD address = FetchAddress();
		Memory[address + 0] = L;
		Memory[address + 1] = H;
	} break;
	case STA_a16:
	{
		DWORD address = FetchAddress();
		Memory[address] = A;
	} break;
	case STAX_B:
	{
		Memory[BC] = A;
	} break;
	case STAX_D:
	{
		Memory[DE] = A;
	} break;
	case STC:
	{
		Flag.CY = true;
	} break;
	case SUB_A:
	{
		Sub(A, A);
		UpdateZSP(A);
	} break;
	case SUB_B:
	{
		Sub(A, B);
		UpdateZSP(A);
	} break;
	case SUB_C:
	{
		Sub(A, C);
		UpdateZSP(A);
	} break;
	case SUB_D:
	{
		Sub(A, D);
		UpdateZSP(A);
	} break;
	case SUB_E:
	{
		Sub(A, E);
		UpdateZSP(A);
	} break;
	case SUB_H:
	{
		Sub(A, H);
		UpdateZSP(A);
	} break;
	case SUB_L:
	{
		Sub(A, L);
		UpdateZSP(A);
	} break;
	case SUB_M:
	{
		Sub(A, Memory[HL]);
		UpdateZSP(A);
	} break;
	case SUI_d8:
	{
		WORD d8 = Fetch();
		Sub(A, d8);
		UpdateZSP(A);
	} break;
	case SBB_A:
	{
		Sbb(A, A);
		UpdateZSP(A);
	} break;
	case SBB_B:
	{
		Sbb(A, B);
		UpdateZSP(A);
	} break;
	case SBB_C:
	{
		Sbb(A, C);
		UpdateZSP(A);
	} break;
	case SBB_D:
	{
		Sbb(A, D);
		UpdateZSP(A);
	} break;
	case SBB_E:
	{
		Sbb(A, E);
		UpdateZSP(A);
	} break;
	case SBB_H:
	{
		Sbb(A, H);
		UpdateZSP(A);
	} break;
	case SBB_L:
	{
		Sbb(A, L);
		UpdateZSP(A);
	} break;
	case SBB_M:
	{
		Sbb(A, Memory[HL]);
		UpdateZSP(A);
	} break;
	case SBI_d8:
	{
		WORD d8 = Fetch();
		Sbb(A, d8);
		UpdateZSP(A);
	} break;
	case XCHG:
	{
		DWORD tmp = HL;
		HL = DE;
		DE = tmp;
	} break;
	case XTHL:
	{
		DWORD tmp = HL;
		HL = Pop();
		Push(tmp);
	} break;
	case XRA_A:
	{
		A ^= A;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_B:
	{
		A ^= B;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_C:
	{
		A ^= C;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_D:
	{
		A ^= D;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_E:
	{
		A ^= E;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_H:
	{
		A ^= H;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_L:
	{
		A ^= L;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRA_M:
	{
		A ^= Memory[HL];
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	case XRI_d8:
	{
		WORD d8 = Fetch();
		A ^= d8;
		UpdateZSP(A);
		Flag.AC = 0;
		Flag.CY = 0;
	} break;
	default:
	{
		// TODO: log this correctly
		std::cout << "Undefined opcode \'0x" << std::hex << (int)opcode << std::dec << "\'" << std::endl;
		assert(false, "Undefined opcode");
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

