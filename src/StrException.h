#pragma once
#include <string>

/**
 * \brief 异常类
 */
struct str_exception
{
	std::string start;
	const wchar_t* error;

	str_exception(std::string start, const wchar_t* error);
};
