#include "Compiler.h"
#include "Parser/Parser.h"
#include <iostream>
#include <map>
using namespace std;

std::vector<std::string> stringToTokenVector(const std::string source)
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
void toUpper(std::string& source)
{
	for (int i = 0; i < source.size(); i++)
	{
		source[i] = toupper(source[i]);
	}
}
void Compiler::Compile(std::string source, const int offset)
{
	std::vector<uint8_t> tmp;
	// NAME; CALLED FROM; REFS TO
	std::map<std::string, int> labels;
	std::vector<std::tuple<std::tuple<int, std::string>, int>> unrefLabels;
	//reset errors
	compileErrors.ClearMessages();
	errorOccured = false;


	toUpper(source);
	auto tokens = stringToTokenVector(source + '\n');
	for (int i = 0; i < tokens.size(); i++)
	{
		try
		{
			Parse(tokens[i], tmp, labels, unrefLabels, offset, i);
		}
		catch (std::string ex)
		{
			errorOccured = true;
			compileErrors.WriteError(i + 1, ex);
		}
	}

	if (unrefLabels.size() != 0)
	{

		for (auto &label : unrefLabels)
		{
			try
			{
				UnknownLabelLeft(std::get<1>(std::get<0>(label)));
			}
			catch (const std::string ex)
			{
				errorOccured = true;
				compileErrors.WriteError(std::get<0>(std::get<0>(label)), ex);
			}
		}
	}
	else if(!errorOccured)
	{
		resultBinary = tmp;
	}
}
void CompileError::WriteError(const int line, const std::string msg)
{
	std::tuple<int, std::string> err{line, msg};
	messages.push_back(err);
}
