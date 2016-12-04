#include "StrException.h"

str_exception::str_exception(std::string start, const wchar_t* error): start(start), error(error)
{ }
