#include "Parser.h"
#include "Register/Register.h"


bool isHexLetterOrNumber(char c)
{
	if (c == 'A' || c == 'B' || c == 'D' || c == 'E' || c == 'F' || isdigit(c))
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

bool isMark(std::string source)
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
