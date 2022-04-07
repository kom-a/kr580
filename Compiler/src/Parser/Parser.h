#pragma once
#include<vector>
#include<string>
#include<map>

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


///checks if argument is register
bool isRegister(const std::string& source);
///checks if argument is data8
bool isData8(const std::string& source);
///checks if argument is data16
bool isData16(const std::string& source);
///checks if argument us mark
bool isLabel(const std::string& source);

///Splits given string by specified character
std::vector<std::string> tokenizeCommand(const std::string& source);
void UnknownLabelLeft(std::string);

///Parses given string and returns fills byte array
void Parse(std::string source, std::vector<uint8_t>& byteArray, std::map<std::string, int>& labels, std::vector<std::tuple<std::string, int>>& unrefLabels, const int& offset);  //offset is the position where code starts