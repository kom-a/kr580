#pragma once

#include <string>
#include <cinttypes>
#include "ISA.h"
#include "Parser/Parser.h"

struct CompileError
{
	size_t Line;
	size_t Pos;
	std::string Message;
};

class Compiler 
{
public:
	void Compile(std::string source);
};
