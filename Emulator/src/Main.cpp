#include <iostream>
#include <vector>
#include "KR580VM80A.h"
#include "ISA.h"

int main()
{
	std::vector<WORD> program =
	{
		MVI_C_d8, 0x08,
		LXI_H_d16, 0x00, 0x09,
		LXI_D_d16, 0x00, 0x0A,
		MOV_A_M,
		SUI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x08, 0x08,
		MVI_C_d8, 0x08,
		MOV_A_M,
		ADI_d8, 0x03,
		STAX_D,
		INX_H,
		INX_D,
		DCR_C,
		JNZ_a16, 0x14, 0x08,
		RST_1
	};

	KR580VM80A kr;
	kr.LoadProgram(program);

	while (true)
		kr.Step();

	return 0;
}
