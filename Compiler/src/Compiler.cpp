#include "Compiler.h"
#include "Parser/Parser.h"
#include<iostream>
#include<map>
using namespace std;

std::vector<std::string> stringToTokenVector(std::string source)
{
	std::vector<std::string> res;
	int i = 0;
	char c = source[i];
	std::string token;
	while (c != '\0')
	{
		if (c != '\n')
			token += c;
		else
		{
			token += c;
			res.push_back(token);
			token = "";
		}
		i++;
		c = source[i];
	}
	return res;
}

void toLower(std::string& source)
{
	for (int i = 0; i < source.size(); i++)
	{
		source[i] = tolower(source[i]);
	}
}
void Compiler::Compile(std::string source, int offset)
{
	std::vector<uint8_t> tmp;
	std::map<std::string, int> labels;
	//reset errors
	compileErrors.ClearMessages();
	errorOccured = false;


	toLower(source);
	auto tokens = stringToTokenVector(source + '\n');
	std::vector<uint8_t> bytes;
	for (int i = 0; i < tokens.size(); i++)
	{
		try
		{
			bytes = Parse(tokens[i], labels, tmp.size(), offset);
		}
		catch (std::string ex)
		{
			errorOccured = true;
			compileErrors.WriteError(i + 1, ex);
		}

		for (auto byte : bytes)
		{
			tmp.push_back(byte);
		}
	}
	if (!errorOccured)
		resultBinary = tmp;
}

void CompileError::WriteError(int line, std::string message)
{
	std::tuple<int, std::string> err{line, message};
	messages.push_back(err);
}
