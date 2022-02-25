#include <iostream>
#include <vector>
#include "KR580VM80A.h"
#include "ISA.h"

int main()
{
	std::vector<WORD> program =
	{
		MVI_C_d8, 0xFF,
		INR_C,
		RST_1
	};

	KR580VM80A kr;
	kr.LoadProgram(program);

	std::cout << "Hello world" << std::endl;

	while (true)
		kr.Step();

	return 0;
}
