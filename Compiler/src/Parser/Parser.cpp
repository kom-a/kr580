#include "Parser.h"
#include "Register/Register.h"
#include "Command/Command.h"
#include "ErrorHandler/CompilerErrorHandler.h"
#include <iostream>

bool isHexLetterOrNumber(char c)
{
	if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || isdigit(c))
	{
		return true;
	}
	return false;
}

bool isRegister(const std::string& source)
{
	for (std::string reg : Register)
	{
		if (source == reg)
			return true;
	}
	return false;
}

bool startsWithPrefix(const std::string& source) {
	// here whole string is already uppercase
	if (source[0] == '0' && source[1] == 'X')
		return true;
	else
		return false;

}
	
bool isData8(const std::string& source)
{
	if (source.size() != 4)
		return false;

	if (!startsWithPrefix(source))
		return false;

	// number start index without prefix
	int i = 2;
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

bool isData16(const std::string& source)
{
	if (source.size() != 6)
		return false;

	if (!startsWithPrefix(source))
		return false;

	// number start index without prefix
	int i = 2;
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

bool isLabel(const std::string& source)
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
					RaiseError(CompilerError::INVALID_LABEL, "unexpected char " + quote(source[i + 1]) + " after label");
				}
			}
			i++;
			c = source[i];
		}
	}
	else
	{
		RaiseError(CompilerError::INVALID_LABEL, "label cannot start with " + quote(c));
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

std::vector<std::string> tokenizeCommand(const std::string& source)
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

/// Deletes multiple spaces
void deleteMultipleSpaces(std::string& source)
{
	// turns multi spaces into one space 
	for (int i = source.size() - 1; i > 0; i--)
	{
		if (source[i] == ' ' && source[i] == source[i - 1])
		{
			source.erase(source.begin() + i);
		}
	}
	// deletes leading space if there was some leading whitespaces 
	if (source[0] == ' ')
		source.erase(source.begin());
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

bool isUnrefLabel(std::string label, std::vector<std::tuple<std::tuple<int, std::string>, int>>& unrefLabels)
{
	for (auto ln : unrefLabels)
	{
		if (std::get<1>(std::get<0>(ln)) == label)
		{
			return true;
		}
	}
	return false;
}

void insertUnrefLabelAddresses(
	std::string label,
	int16_t address,
	std::vector<uint8_t>& arr,
	std::map<std::string, int>& labels,
	std::vector<std::tuple<std::tuple<int, std::string>, int>>& unrefLabels, int offset)
{
	for(int i = 0; i < unrefLabels.size(); i++)
	{
		if (std::get<1>(std::get<0>(unrefLabels[i])) == label)
		{
			arr[std::get<1>(unrefLabels[i])] = ((address + offset) & 0x000000ff);
			arr[std::get<1>(unrefLabels[i]) + 1] = ((address + offset) & 0x0000ff00) >> 8;
			unrefLabels.erase(unrefLabels.begin() + i);
			i--;
		}
	}
}

void UnknownLabelLeft(std::string label)
{
	RaiseError(CompilerError::UNDEFINED_LABEL, "unknown label " + quote(label));
}

void processLabelArg(
	std::vector<uint8_t>& byteArray,
	std::map<std::string, int>& labels,
	std::vector<std::tuple<std::tuple<int, std::string>, int>>&unrefLabels,
	std::string label,
	int offset,
	int line
) {
	if (labels.find(label) != labels.end())
	{
		int resAddr = labels[label] + offset;
		byteArray.push_back(resAddr & 0x000000ff);
		byteArray.push_back((resAddr & 0x0000ff00) >> 8);
	}
	else if (BuilInLabels.find(label) != BuilInLabels.end())
	{
		int resAddr = BuilInLabels.at(label);
		byteArray.push_back(resAddr & 0x000000ff);
		byteArray.push_back((resAddr & 0x0000ff00) >> 8);
	}
	else
	{
		unrefLabels.push_back({ {line + 1, std::string(label)}, (int)byteArray.size() });
		byteArray.push_back(0);
		byteArray.push_back(0);
		// do not raise error because for now label is probably below
		//RaiseError(CompilerError::UNDEFINED_LABEL, "unknown label " + quote(tokens[1]));
	}
}

bool isCompilerCommand(std::string command)
{
	for (auto comm : CompilerCommandPrototypes) {
		if (std::get<0>(comm) == command)
		{
			return true;
		}
	}
	return false;
}

void processCompilerCommand(
	std::vector<uint8_t>& byteArray,
	std::vector<std::string> tokens
)
{
	int size = tokens.size();
	std::string command = tokens[0];
	int commandCode = getCompilerBuiltInCommand(tokens[0]);
	switch (commandCode) {
		case DW:
			if (size != 2)
			{
				RaiseError(CompilerError::INVALID_ARGUMENT_NUM, "expected 1 argument, got " + size);
			}
			else if (!isData8(tokens[1]))
			{
				RaiseError(CompilerError::INVALID_ARGUMENT, "expected 1 byte argument, got " + quote(tokens[1]));
			}
			else
			{
				byteArray.push_back((uint8_t)strtol(tokens[1].substr(2, 2).c_str(), nullptr, 16));
			}
			break;
		case DD:
			if (size != 2)
			{
				RaiseError(CompilerError::INVALID_ARGUMENT_NUM, "expected 1 argument, got " + size);
			}
			else if (!isData16(tokens[1]))
			{
				RaiseError(CompilerError::INVALID_ARGUMENT, "expected 2 bytes argument, got " + quote(tokens[1]));
			}
			else
			{
				// secons byte 
				byteArray.push_back((uint8_t)strtol(tokens[1].substr(4).c_str(), nullptr, 16));
				// first byte
				byteArray.push_back((uint8_t)strtol(tokens[1].substr(2, 2).c_str(), nullptr, 16));
			}
			break;
	}
}

void Parse(
	std::string source,
	std::vector<uint8_t>& byteArray,
	std::map<std::string, int>& labels,
	std::vector<std::tuple<std::tuple<int, std::string>, int>>& unrefLabels,
	const int& offset,
	const int line
)
{
	std::string errMessage = "";
	normalize(source);
	if (notJustSpaces(source))
	{
		auto tokens = tokenizeCommand(source);
		if (isCompilerCommand(tokens[0]))
		{
			processCompilerCommand(byteArray, tokens);
			return;
		}
		int size = tokens.size();
		if (size > 3)				//error
		{
			RaiseError(CompilerError::INVALID_ARGUMENT_NUM, "more than 2 args gived");
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
				RaiseError(CompilerError::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[1]));

			if (isRegister(tokens[2]))
				argType2 = "reg";
			else if (isData8(tokens[2]))
				argType2 = "d8";
			else if (isData16(tokens[2]))
				argType2 = "d16";
			else if (isLabel(tokens[2]))
				argType2 = "label";
			else
				RaiseError(CompilerError::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[2]));

			if (!prototypeExists(tokens[0], argType1, argType2))
			{
				RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType1 + " " + argType2));
			}
			else
			{
				if (argType1 == "reg")
					argType1 = tokens[1];

				if (argType2 == "reg")
					argType2 = tokens[2];

				//PUSH BYTES
				int8_t code = getCommandOpcode(tokens[0] + "_" + (argType1 == "label" ? "d16" : argType1) + "_" + (argType2 == "label" ? "d16" : argType2));
				if (code == -1)
					RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType1 + " " + argType2));
				byteArray.push_back(code);
				
				if(argType1 == "d8")
					byteArray.push_back((uint8_t)strtol(tokens[1].substr(2).c_str(), nullptr, 16));
				else if (argType1 == "d16")
				{
					// secons byte 
					byteArray.push_back((uint8_t)strtol(tokens[1].substr(4).c_str(), nullptr, 16));
					// first byte
					byteArray.push_back((uint8_t)strtol(tokens[1].substr(2, 2).c_str(), nullptr, 16));
				}
				else if (argType1 == "label") {
					processLabelArg(byteArray, labels, unrefLabels, tokens[1], offset, line);
				}

				if (argType2 == "d8")
					byteArray.push_back((uint8_t)strtol(tokens[2].substr(2).c_str(), nullptr, 16));
				else if (argType2 == "d16")
				{
					byteArray.push_back((uint8_t)strtol(tokens[2].substr(4).c_str(), nullptr, 16));
					byteArray.push_back((uint8_t)strtol(tokens[2].substr(2, 2).c_str(), nullptr, 16));
				}
				else if (argType2 == "label") {
					processLabelArg(byteArray, labels, unrefLabels, tokens[2], offset, line);
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
				RaiseError(CompilerError::INVALID_ARGUMENT, std::string("unknown argument type of ") + quote(tokens[1]));

			if (!prototypeExists(tokens[0], argType, ""))
			{
				RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
			}
			else
			{
				if (argType == "reg")
				{
					argType = tokens[1];
					int8_t code = getCommandOpcode(tokens[0] + "_" + argType);
					if (code == -1)
						RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
					byteArray.push_back(code);

				}				
				else
				{
					int8_t code = getCommandOpcode(tokens[0] + "_" + (argType == "label"? "d16":argType));
					if (code == -1)
						RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command prototype " + quote(tokens[0] + " " + argType));
					byteArray.push_back(code);
				}

				if (argType == "d8")
					byteArray.push_back((uint8_t)strtol(tokens[1].c_str(), nullptr, 16));
				else if (argType == "d16")
				{
					byteArray.push_back((uint8_t)strtol(tokens[1].substr(4).c_str(), nullptr, 16));
					byteArray.push_back((uint8_t)strtol(tokens[1].substr(2, 2).c_str(), nullptr, 16));
				}
				else if (argType == "label")
				{
					processLabelArg(byteArray, labels, unrefLabels, tokens[1], offset, line);
				}
				else if (isRegister(argType));
				else
				{
					RaiseError(CompilerError::INVALID_ARGUMENT, "invalid argument " + quote(tokens[1]));
				}
					
			}
		}
		else if (size == 1)		// 0 args command
		{
			if (!prototypeExists(tokens[0], "", ""))
			{
				if (tokens[0].find(':') != -1)
				{
					parseLabel(tokens[0]);
					std::string labelName = tokens[0].substr(0, tokens[0].size() - 1);
					if (labels.find(labelName) == labels.end())
					{
						labels.insert({ labelName, byteArray.size()});
						if (isUnrefLabel(labelName, unrefLabels))
						{
							insertUnrefLabelAddresses(labelName, byteArray.size(), byteArray, labels, unrefLabels, offset);
						}
					}
					else
					{
						RaiseError(CompilerError::REDEFINED_LABEL, "label " + quote(tokens[0]) + "is already defined");
					}
				}
				else
				{
					RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command " + quote(tokens[0]));
				}
			}
			else
			{
				int8_t code = getCommandOpcode(tokens[0]);
				if (code == -1)
					RaiseError(CompilerError::UNKNOWN_COMMAND, "unknown command " + quote(tokens[0]));
				byteArray.push_back(code);
			}
		}
		else
		{
			RaiseError(CompilerError::UNEXPECTED_ERROR, "unknown error");
		}
	}
}
