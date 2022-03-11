#pragma once
#include<vector>
#include<string>
#include<tuple>

enum class ErrorType
{
	UNKNOWN_COMMAND = 1,
	INVALID_ARGUMENT,
	INVALID_ARGUMENT_NUM,
	UNEXPECTED_ERROR,
	INVALID_LABEL,
	UNDEFINED_LABEL,
};

static std::vector<std::tuple<ErrorType, std::string>> ErrMessages
{
	{ ErrorType::UNKNOWN_COMMAND, "ERROR unknown command: " },
	{ ErrorType::INVALID_ARGUMENT, "ERROR invalid argument: " },
	{ ErrorType::INVALID_ARGUMENT_NUM, "ERROR invalid argument number: " },
	{ ErrorType::UNEXPECTED_ERROR, "UNEXPECTED ERROR: " },
	{ ErrorType::INVALID_LABEL, "ERROR INVALID LABEL: " },
	{ ErrorType::UNDEFINED_LABEL, "ERROR UNDEFINED LABEL: " },
};


std::string RaiseError(ErrorType, std::string arg);