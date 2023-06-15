#pragma once
#include<vector>
#include<string>
#include<map>


///checks if argument is register
bool isRegister(const std::string& source);
///checks if argument is data8
bool isData8(const std::string& source);
///checks if argument is data16
bool isData16(const std::string& source);
///checks if argument is mark
bool isLabel(const std::string& source);

///Splits given string by specified character
std::vector<std::string> tokenizeCommand(const std::string& source);
void UnknownLabelLeft(std::string);

///Parses given string and returns byte array
void Parse(
	std::string source,
	std::vector<uint8_t>& byteArray,
	std::map<std::string,
	int>& labels,
	std::vector<std::tuple<std::tuple<int, std::string>,
	int>>& unrefLabels,
	const int& offset,
	const int line
);  //offset is the position where code starts