#pragma once
#include<vector>
#include<string>
#include<tuple>

enum class CompilerError
{
	UNKNOWN_COMMAND = 1,
	INVALID_ARGUMENT,
	INVALID_ARGUMENT_NUM,
	UNEXPECTED_ERROR,
	INVALID_LABEL,
	UNDEFINED_LABEL,
	ALREADY_DEFINED_LABEL,
};

const static std::vector<std::tuple<CompilerError, std::string>> CompileErrMessages
{
	{ CompilerError::UNKNOWN_COMMAND, "COMPILE ERROR UNKNOWN COMMAND: " },
	{ CompilerError::INVALID_ARGUMENT, "COMPILE ERROR INVALID ARGUMENT: " },
	{ CompilerError::INVALID_ARGUMENT_NUM, "COMPILE ERROR INVALID ARGUMENT NUMBER: " },
	{ CompilerError::UNEXPECTED_ERROR, "UNEXPECTED COMPILE ERROR: " },
	{ CompilerError::INVALID_LABEL, "COMPILE ERROR INVALID LABEL: " },
	{ CompilerError::UNDEFINED_LABEL, "COMPILE ERROR UNDEFINED LABEL: " },
	{ CompilerError::ALREADY_DEFINED_LABEL, "COMPILE ERROR ALREADY DEFINED LABEL: " },
};

static std::string quote(std::string str)
{
	return "\'" + str + "\'";
}
static std::string quote(char c)
{
	return (std::string)"\'" + c + "\'";
}

std::string RaiseError(CompilerError, std::string arg);