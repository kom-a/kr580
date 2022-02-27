#pragma once

#include <string>
#include <cinttypes>

struct CompileError
{
	size_t Line;
	size_t Pos;
	std::string Message;
};

bool Compile(uint8_t* memory);

CompileError GetError();
