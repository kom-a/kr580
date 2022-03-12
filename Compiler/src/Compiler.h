#pragma once

#include <string>
#include <cinttypes>
#include <vector>
#include <tuple>

struct CompileError
{
public:
	std::vector<std::tuple<int, std::string>> messages;
	inline void ClearMessages() { messages.clear(); };
	void WriteError(const int line, const std::string message);
};

class Compiler 
{
public:
	bool errorOccured = false;
	CompileError compileErrors;
	std::vector<uint8_t> resultBinary;
	void Compile(std::string source,  const int offset);
};
