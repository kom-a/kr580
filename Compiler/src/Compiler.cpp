#include "Compiler.h"
#include<iostream>
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
void Compiler::Compile(std::string source)
{
	std::vector<uint8_t> tmp;

	//reset errors
	compileErrors.ClearMessages();
	errorOccured = false;


	toLower(source);
	auto tokens = stringToTokenVector(source);
	for (int i = 0; i < tokens.size(); i++)
	{
		try
		{
			tmp.push_back(Parse(tokens[i]));
		}
		catch (const std::exception& ex)
		{
			compileErrors.RaiseError(i + 1, ex.what());
		}
	}
	if (!errorOccured)
		resultBinary = tmp;
}

void CompileError::RaiseError(int line, std::string message)
{
	std::tuple<int, std::string> err{line, message};
	messages.push_back(err);
}
