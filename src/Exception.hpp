#pragma once
#include <string>
/**
 * \brief 异常类
 */

struct Exception
{
	std::string Start;
	const wchar_t* Error;

	Exception(std::string aStart, const wchar_t* aError);
};
