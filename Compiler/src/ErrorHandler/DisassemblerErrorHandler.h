#pragma once
#include<vector>
#include<string>
#include<tuple>

enum class DisassemblerError
{
	UNKNOWN_OPCODE = 1,
	INVALID_ARG_NUM,
};

const static std::vector<std::tuple<DisassemblerError, std::string>> DisassembleErrMessages
{
	{ DisassemblerError::UNKNOWN_OPCODE, "DISASSEMBLE ERROR UNKNOWN OPCODE: " },
	{ DisassemblerError::INVALID_ARG_NUM, "DISASSEMBLE ERROR INVALID ARGUMENT NUMBER: " },
};


static std::string quote(std::string str)
{
	return "\'" + str + "\'";
}
static std::string quote(char c)
{
	return (std::string)"\'" + c + "\'";
}

std::string RaiseError(DisassemblerError, std::string arg);