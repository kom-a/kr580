#include "ErrorHandler/DisassemblerErrorHandler.h"

std::string RaiseError(DisassemblerError errType, std::string addictionalInfo)
{
	for (auto err : DisassembleErrMessages)
	{
		if (std::get<0>(err) == errType)
		{
			throw(std::get<1>(err) + " " + addictionalInfo);
		}
	}
}