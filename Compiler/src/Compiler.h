#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <tuple>
#include "Parser/Parser.h"

struct CompileError
{
public:
	std::vector<std::tuple<int, std::string>> messages;
	inline void ClearMessages() { messages.clear(); };
	void RaiseError(int line, std::string message);
};

class Compiler 
{
public:
	bool errorOccured;
	CompileError compileErrors;
	std::vector<uint8_t> resultBinary;
	void Compile(std::string source);
};
