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
};

static std::vector<std::tuple<ErrorType, std::string>> ErrMessages
{
	{ ErrorType::UNKNOWN_COMMAND, "ERROR unknown command: " },
	{ ErrorType::INVALID_ARGUMENT, "ERROR invalid argument: " },
	{ ErrorType::INVALID_ARGUMENT_NUM, "ERROR invalid argument number: " },
	{ ErrorType::UNEXPECTED_ERROR, "UNEXPECTED ERROR: " },
};


std::string RaiseError(ErrorType, std::string arg);