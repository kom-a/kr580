#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <map>

struct DisassembleErrors
{
public:
	std::tuple<std::string, std::string> message;
	inline void ClearMessages() { message = {}; };
	void WriteError(const std::string line, const std::string message);
};


class Disassembler
{
private:
	std::vector<std::string> GetMnemonics(const std::vector<uint8_t>& byteArr, std::map<uint16_t, std::string>& references, int& errAddr, const int& offset);
public:
	bool errorOccured = false;
	DisassembleErrors disassembleError;
	std::vector<std::string> mnemonics;
	void Disassemble(const std::vector<uint8_t>&, const int offset);
};
