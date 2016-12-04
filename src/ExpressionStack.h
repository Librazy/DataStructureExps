#pragma once

#ifndef ExpressionStack_disabled

#ifndef ExpressionStack_defined

// ReSharper disable CppUnusedIncludeDirective
#include<memory>
#include<string>
#include<sstream>
#include<stack>
#include <map>


struct expression_stack{
	enum class precede
	{
		L = -1,
		E = 0,
		H = 1,
		F = 2,

	};
	enum class op{
		Plus           = 0,
		Minus          = 1,
		Multiplication = 2,
		Division       = 3,
		LBracket       = 4,
		RBracket       = 5,
		Sharp          = 6,
	};
	
	static const std::map<char, op> operator_map;

	static const precede precede_table[7][7];

	static double eval(std::string exp);
};


#define ExpressionStack_defined

#endif

#endif