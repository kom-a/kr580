#include "ErrorHandler.h"

std::string RaiseError(ErrorType errType, std::string addictionalInfo)
{
	for (auto err : ErrMessages)
	{
		if (std::get<0>(err) == errType)
		{
			throw(std::get<1>(err) + " " + addictionalInfo);
		}
	}
}
