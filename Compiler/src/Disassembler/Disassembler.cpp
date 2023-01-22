#include <KR580.h>

#include "Disassembler/Disassembler.h"
#include <sstream>
#include <iomanip>
#include "ErrorHandler/DisassemblerErrorHandler.h"
#include "Command/Command.h"

using namespace KR580;

OpcodeInfo_t getArgInfo(uint8_t opcode)
{
	if (OpcodeInfo.find((Opcode)opcode) != OpcodeInfo.end())
	{
		return OpcodeInfo.at((Opcode)opcode);
	}
}

std::string toHexString(const uint8_t& byte) {
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	ss << std::hex << std::setw(2) << static_cast<int>(byte);

	return ss.str();
}

std::string toHexString(const uint16_t& bytes) {
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	ss << std::hex << std::setw(4) << static_cast<int>(bytes);

	return ss.str();
}

void strToUpper(std::string& source)
{
	for (int i = 0; i < source.size(); i++)
	{
		source[i] = toupper(source[i]);
	}
}

std::string getBuiltInLabel(uint16_t addr)
{
	for (auto label : BuilInLabels)
	{
		if (label.second == addr)
			return label.first;
	}
	return "";
}

bool referenceExists(int addr, std::map<uint16_t, std::string>& references)
{
	for (auto ref : references)
	{
		if(std::get<0>(ref) == addr)
			return true;
	}
	return false;
}


std::vector<std::string> Disassembler::GetMnemonics(const std::vector<uint8_t>& byteArr, std::map<uint16_t, std::string>& references, int& errAddr, const int& offset)
{
	std::vector<std::string> res;
	for (int i = 0; i < byteArr.size(); i++)
	{
		std::string line;
		std::string argType;
		//Get opcode
		OpcodeInfo_t arg = getArgInfo(byteArr[i]);
		if (arg.Length != 0)
		{
			std::string commnadPrototype = arg.Mnemonic;
			std::string command;
			int ind = commnadPrototype.find("d8");			//data8 arg
			if (ind != -1)
			{
				argType = "d8";
				command = commnadPrototype.substr(0, ind);
				line += command;
			}

			ind = commnadPrototype.find("d16");				//data16 arg
			if (ind != -1)
			{
				argType = "d16";
				command = commnadPrototype.substr(0, ind);
				line += command;
			}

			ind = commnadPrototype.find("a16");				//address arg
			if (ind != -1)
			{
				argType = "a16";
				command = commnadPrototype.substr(0, ind);
				line += command;
			}

			ind = commnadPrototype.find("pp");				//port arg
			if (ind != -1)
			{
				argType = "pp";
				command = commnadPrototype.substr(0, ind);
				line += command;
			}

			if (argType.empty())
			{
				command = commnadPrototype;
				line += command;
			}
			//Get command bytes 
			int j = arg.Length - 1;
			int k = i + j;
			for (k; k > i; k--)
			{
				std::string byte = toHexString(byteArr[k]);
				strToUpper(byte);

				if (argType == "d8")
					line += " " + byte;
				else
					line += byte;
			}

			if (k != i)  //not enough bytes
			{
				errAddr = offset + i;
				RaiseError(DisassemblerError::INVALID_ARG_NUM, "command " + quote(command) + std::string(" expected length is ") + std::to_string(arg.Length - 1) + " bytes but got " + std::to_string(arg.Length - j));
			}
			else
			{
				i += j;
				int ind = commnadPrototype.find(' ');
				if (ind != -1)
				{
					commnadPrototype[ind] = '_';
					if (isLabelArgCommand(commnadPrototype))
					{
						int16_t addr = ((uint16_t)byteArr[i]) << 8;
						addr = addr + (uint16_t)byteArr[i - 1];
						std::string builtInLabel = getBuiltInLabel(addr);

						if (builtInLabel != "")
						{
							line.replace(line.size() - 4, line.size(), builtInLabel);
						}
						else
						{
							if (!referenceExists(addr, references))
							{
								references.insert({ addr, std::string("Label") + std::to_string(references.size()) + ":" });
							}
							line.replace(line.size() - 4, line.size(), ("Label" + std::to_string(references.size() - 1)).c_str());
						}
					}
				}
				
				if (arg.Length != 0)
				{
					res.push_back(line + '\n');
				}
			}
		}
		else
		{
			errAddr = offset + i;
			RaiseError(DisassemblerError::UNKNOWN_OPCODE, "unknown opcode " + quote("0x" + toHexString(byteArr[i])));
		}
	}
		
	return res;
}

int getLabelIndex(const std::vector<uint8_t>& byteArr, uint16_t offset)
{
	int labelIndex = 0;
	for (int i = 0; i < byteArr.size() && i < offset; i++)
	{
		OpcodeInfo_t arg = getArgInfo(byteArr[i]);
		i += arg.Length - 1;
		labelIndex++;
	}
	return labelIndex;
}

void Disassembler::Disassemble(const std::vector<uint8_t>& byteArr, const int offset)
{
	std::vector<std::string> res;
	int errAddr;
	std::map<uint16_t, std::string> references;
	errorOccured = false;
	try
	{
		res = GetMnemonics(byteArr, references, errAddr, offset);
		int proceed = 0;
		for (auto ref : references)
		{
			int ind = uint16_t (ref.first - ((uint16_t)offset));
			int labelIndex = getLabelIndex(byteArr, ind) + proceed;
			proceed++;
			res.insert(res.begin() + labelIndex, ref.second + "\n");
		}
	}
	catch (const std::string ex)
	{
		errorOccured = true;
		disassembleError.WriteError(toHexString((uint16_t)errAddr), ex);
	}
	mnemonics.clear();
	if (!errorOccured)
	{
		for (auto mnem : res)
		{
			if(mnem != "NOP\n")
				mnemonics.push_back(mnem);
		}
	}
}

void DisassembleErrors::WriteError(const std::string addr, const std::string msg)
{
	std::tuple<std::string, std::string> err{ addr, msg};
	message = err;
}
