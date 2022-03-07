#pragma once
#include<vector>
#include<string>

enum class States
{
	Start = 1,
	Command,
	Arg8,
	Arg16,
	Err,
	Comment,
	Register,
	Label,
};


enum ExpectedArgumentType
{
	Reg = 1,
	Data8,
	Dat
	,
	Label,

};
///checks if argument is register
bool isRegister(std::string source);
///checks if argument is data8
bool isData8(std::string source);
///checks if argument is data16
bool isData16(std::string source);
///checks if argument us mark
bool isMark(std::string source);

///Splits given string by specified character
std::vector<std::string> tokenizeCommand(std::string source);


///Parses given string and returns its opcode
std::vector<uint8_t> Parse(std::string source);