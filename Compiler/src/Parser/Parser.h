#pragma once
#include<string>

///checks if argument is register
bool isRegister(std::string source);
///checks if argument is data8
bool isData8(std::string source);
///checks if argument is data16
bool isData16(std::string source);
///checks if argument us mark
bool isMark(std::string source);