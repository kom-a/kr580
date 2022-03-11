#include "Parser.h"
#include "Register/Register.h"
#include "Command/Command.h"
#include "ErrorHandler/ErrorHandler.h"
#include <iostream>
#include <sstream>

std::string quote(std::string str)
{
	return "\'" + str + "\'";
}
std::string quote(char c)
{
	return (std::string) "\'" + c + "\'";
}
bool isHexLetterOrNumber(char c)
{
	if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || isdigit(c))
	{
		return true;
	}
	return false;
}

bool isRegister(std::string source)
{
	for (std::string reg : Register)
	{
		if (source == reg)
			return true;
	}
	return false;
}

bool isData8(std::string source)
{
	if (source.size() != 2)
		return false;

	int i = 0;
	char c = source[i];
	while (c != '\0')
	{
		if (!isHexLetterOrNumber(c))
			return false;
		i++;
		c = source[i];
	}
	return true;
}

bool isData16(std::string source)
{
	if (source.size() != 4)
		return false;

	int i = 0;
	char c = source[i];
	while (c != '\0')
	{
		if (!isHexLetterOrNumber(c))
			return false;
		i++;
		c = source[i];
	}
	return true;
}

bool isLabel(std::string source)
{
	int i = 0;
	char c = source[i];
	if (isalpha(c) || c == '_')
	{
		i++;
		c = source[i];
		while (c != '\0')
		{
			if (!isalnum(c) && c != '_')
			{
				return false;
			}
			i++;
			c = source[i];
		}
	}
	return true;
}

void parseLabel(std::string source)
{
	int i = 0;
	char c = source[i];
	if (isalpha(c) || c == '_')
	{
		i++;
		c = source[i];
		while (c != '\0')
		{
			if (!isalnum(c) && c != '_')
			{
				if(c == ':' && i != source.size() - 1)
				{
					RaiseError(ErrorType::INVALID_LABEL, "unexpected char " + quote(source[i + 1]) + " after label");
				}
			}
			i++;
			c = source[i];
		}
	}
	else
	{
		RaiseError(ErrorType::INVALID_LABEL, "label cannot start with " + quote(c));
	}
}

std::string deleteAroundSpaces(std::string source)
{
	if (source.size() == 0)
		return source;
	int ind = 0;
	while (source[ind] != '\0' && !isalnum(source[ind]) && source[ind] != '_')
	{
		if (source[ind] == ' ')
		{
			source.erase(source.begin() + ind);
			break;
		}
		ind++;
	}
	ind = source.size() - 1;
	while (source[ind] != '\0' && !isalnum(source[ind]) && source[ind] != '_')
	{
		if (source[ind] == ' ')
		{
			source.erase(source.begin() + ind);
		}
		ind--;
	}
	return source;
}
std::vector<std::string> split(std::string s, char del = ' ')
{
	std::vector<std::string> res;
	int start = 0;
	int end = s.find(del);
	while (end != -1) {
		res.push_back(s.substr(start, end - start));
		start = end + 1;
		end = s.find(del, start);
	}
	res.push_back(s.substr(start, end - start));
	return res;
}

std::vector<std::string> tokenizeCommand(std::string source)
{
	/*
	*	No spaces in string means the string is 0 args command or it's an error, and we just return the token assuming that it's a command
	*	If we can find the space we sould split the string by space, the first part of the result is command, second - arguments
	*	Now we can remove spaces from args part of string and if we can't find a comma means that command has one arg 
		and we just return command and argument. Further we will check if they were correct.
	*	If we can find a comma we should split an argument part of string and return.
	*/	
	std::vector<std::string> res;
	int ind = source.find(' ');
	if (ind == -1)
	{
		res.push_back(source); // No args command or error
	}
	else
	{
		res.push_back(source.substr(0, ind));						//command 
		std::string args = deleteAroundSpaces(source.substr(ind + 1));	//args

		if (args[0] == '\0')			//No args command with trailing space
			return res;
		if (args.find(',') == -1)
		{
			res.push_back(args);   //one argument command
		}
		else
		{
			for (auto token : split(args, ','))
			{
				res.push_back(deleteAroundSpaces(token));			//two args command
			}
		}
	}
	return res;
}

int getLabelAddr(std::string label, std::map<std::string, int>& labels) 
{
	if (labels.find(label) != labels.end())
	{
		return labels[label];
	}
	else if(BuilInLabels.find(label) != BuilInLabels.end())
	{
		return BuilInLabels[label];
	}
	else 
		return -1;
}

/// Deletes multiple spaces
void deleteMultipleSpaces(std::string& source)
{
	for (int i = source.size() - 1; i > 0; i--)
	{
		if (source[i] == ' ' && source[i] == source[i - 1])
		{
			source.erase(source.begin() + i);
		}
	}
}
///Replaces each whitespace to space
void wspaceToSpace(std::string& source)
{
	for (int i = 0; i < source.size(); i++)
	{
		if (source[i] == '\n' || source[i] == '\t' || source[i] == '\r' || source[i] == '\f' || source[i] == '\v')
			source[i] = ' ';
	}
}

bool notJustSpaces(std::string& source)
{
	for (char c : source)
	{
		if (c != ' ')
			return true;
	}
	return false;
}

void normalize(std::string& source)
{
	int semicolonInd = source.find(';');
	if (semicolonInd != -1)
		source = source.substr(0, semicolonInd);
	wspaceToSpace(source);
	deleteMultipleSpaces(source);
}

std::vector<uint8_t> Parse(std::string source, std::map<std::string, int>& labels, int currentBytesCount, int offset)
{
	std::string errMessage = "";
	std::vector<uint8_t> res;
	normalize(source);
	if (notJustSpaces(source))
	{
		auto tokens = tokenizeCommand(source);
		int size = tokens.size();
		if (size > 3)				//error
		{
			RaiseError(ErrorType::INVALID_ARGUMENT_NUM, "more than 2 args gived");
		}
		else if (size == 3)  //2 args command
		{

			std::string argType1;
			std::string argType2;

			if (isRegister(tokens[1]))
				argType1 = "reg";
			else if (isData8(tokens[1]))
				argType1 = "d8";
			else if (isData16(tokens[1]))
				argType1 = "d16";
			else
				RaiseError(ErrorType::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[1]));

			if (isRegister(tokens[2]))
				argType2 = "reg";
			else if (isData8(tokens[2]))
				argType2 = "d8";
			else if (isData16(tokens[2]))
				argType2 = "d16";
			else
				RaiseError(ErrorType::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[2]));

			if (!protorypeExists(tokens[0], argType1, argType2))
			{
				RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType1 + " " + argType2));
			}
			else
			{
				if (argType1 == "reg")
					argType1 = tokens[1];

				if (argType2 == "reg")
					argType2 = tokens[2];

				//PUSH BYTES
				int8_t code = getCommandOpcode(tokens[0] + "_" + argType1 + "_" + argType2);
				if (code == -1)
					RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType1 + " " + argType2));
				res.push_back(code);
				
				if(argType1 == "d8")
					res.push_back((uint8_t)strtol(tokens[1].c_str(), nullptr, 16));
				else if (argType1 == "d16")
				{
					res.push_back((uint8_t)strtol(tokens[1].substr(2).c_str(), nullptr, 16));
					res.push_back((uint8_t)strtol(tokens[1].substr(0, 2).c_str(), nullptr, 16));
				}

				if (argType2 == "d8")
					res.push_back((uint8_t)strtol(tokens[2].c_str(), nullptr, 16));
				else if (argType2 == "d16")
				{
					res.push_back((uint8_t)strtol(tokens[2].substr(2).c_str(), nullptr, 16));
					res.push_back((uint8_t)strtol(tokens[2].substr(0, 2).c_str(), nullptr, 16));
				}

			}
		}
		else if (size == 2)		// 1 arg command
		{
			std::string argType;
			if (isRegister(tokens[1]))
				argType = "reg";
			else if (isData8(tokens[1]))
				argType = "d8";
			else if (isData16(tokens[1]))
				argType = "d16";
			else if (isLabel(tokens[1]))
				argType = "label";
			else
				RaiseError(ErrorType::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[1]));

			if (!protorypeExists(tokens[0], argType, ""))
			{
				RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
			}
			else
			{
				if (argType == "reg")
				{
					argType = tokens[1];
					int8_t code = getCommandOpcode(tokens[0] + "_" + argType);
					if (code == -1)
						RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
					res.push_back(code);

				}				
				else
				{
					int8_t code = getCommandOpcode(tokens[0] + "_" + (argType == "label"? "d16":argType));
					if (code == -1)
						RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
					res.push_back(code);
				}

				if (argType == "d8")
					res.push_back((uint8_t)strtol(tokens[1].c_str(), nullptr, 16));
				else if (argType == "d16")
				{
					res.push_back((uint8_t)strtol(tokens[1].substr(2).c_str(), nullptr, 16));
					res.push_back((uint8_t)strtol(tokens[1].substr(0, 2).c_str(), nullptr, 16));
				}
				else if (argType == "label")
				{
					int addr = getLabelAddr(tokens[1], labels);
					if (addr != -1)
					{
						std::stringstream ss;
						ss << std::hex << (addr + offset);
						argType = ss.str();
						res.push_back((uint8_t)strtol(argType.substr(2).c_str(), nullptr, 16));
						res.push_back((uint8_t)strtol(argType.substr(0, 2).c_str(), nullptr, 16));
					}
					else
					{
						RaiseError(ErrorType::UNDEFINED_LABEL, "unknown label " + quote(tokens[1]));
					}
				}
				else if (isRegister(argType));
				else
				{
					RaiseError(ErrorType::INVALID_ARGUMENT, "invalid argument " + quote(tokens[1]));
				}
					
			}
		}
		else if (size == 1)		// 0 args command
		{
			if (!protorypeExists(tokens[0], "", ""))
			{
				if (tokens[0].find(':') != -1)
				{
					parseLabel(tokens[0]);
					labels.insert({tokens[0].substr(0, tokens[0].size() - 1), currentBytesCount + 1});
				}
				else
				{
					RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command " + quote(tokens[0]));
				}
			}
			else
			{
				int8_t code = getCommandOpcode(tokens[0]);
				if (code == -1)
					RaiseError(ErrorType::UNKNOWN_COMMAND, "unknown command " + quote(tokens[0]));
				res.push_back(code);
			}
		}
		else
		{
			RaiseError(ErrorType::UNEXPECTED_ERROR, "unknown error");
		}
	}
	return res;
}
