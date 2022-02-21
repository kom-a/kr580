#include <iostream>
#include <vector>
#include "Compiler.h"
#include "KR580VM80A.h"
#include "ISA.h"

int main()
{
	std::cout << "Hello world!" << std::endl;

	std::vector<WORD> program =
	{
		MVI_C_d8, 0x04,
		LDA_a16, 0x01, 0x09,
		MOV_B_A,
		LDA_a16, 0x02, 0x09,
		ADD_B,
		DCR_C,
		JNZ_a16, 0x05, 0x08,
		STA_a16, 0x06, 0x09,
		RST_1
	};

	KR580VM80A kr;
	kr.LoadProgram(program);


	return 0;
}
