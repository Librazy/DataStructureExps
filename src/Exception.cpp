#include "Exception.hpp"

Exception::Exception(std::string aStart, const wchar_t* aError)
{
	Start = aStart;
	Error = aError;
}
