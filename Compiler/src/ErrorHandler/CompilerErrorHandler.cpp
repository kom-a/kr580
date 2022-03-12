#include "ErrorHandler/CompilerErrorHandler.h"

std::string RaiseError(CompilerError errType, std::string addictionalInfo)
{
	for (auto err : CompileErrMessages)
	{
		if (std::get<0>(err) == errType)
		{
			throw(std::get<1>(err) + " " + addictionalInfo);
		}
	}
}