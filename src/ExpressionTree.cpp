#include "ExpressionTree.hpp"
#include "Exception.hpp"

#ifdef ExpressionTree_defined

Expression::Expression()
{
	IsLeft = false;
}

NumberExpression::NumberExpression(int number)
{
	Value = number;
}

double NumberExpression::Eval()
{
	return Value;
}

BinaryExpression::BinaryExpression(BinaryOperator Op, std::unique_ptr<Expression>&& First, std::unique_ptr<Expression>&& Second) :
	First(move(First)), Second(move(Second)), Op(Op)
{ }

double BinaryExpression::Eval()
{
	auto lhs = First->Eval();
	auto rhs = Second->Eval();
	
	switch (Op) {
	case BinaryOperator::Plus:
		return lhs + rhs;
	case BinaryOperator::Minus:
		return lhs - rhs;
	case BinaryOperator::Multiplication:
		return lhs * rhs;
	case BinaryOperator::Division:
		return lhs / rhs;
	default: throw Exception("", L"错误的操作符");
	}
}

bool Is(std::stringstream& Stream, const char Text)
{
	auto Read = Stream.tellg();
	while (Stream.peek() == ' ')Stream.get();
	
	if (Stream.peek() == Text) {
		Stream.get();
		return true;
	}
	Stream.seekg(Read);
	return false;
}

std::unique_ptr<NumberExpression> GetNumber(std::stringstream& Stream)
{
	auto Result = 0;
	auto GotNumber = false;
	while (Stream.peek() == ' ')Stream.get();
	while (true) {
		auto c = Stream.peek();
		if ('0' <= c && c <= '9') {
			Result = Result * 10 + (c - '0');
			GotNumber = true;
			Stream.get();
		} else { break; }
	}
	if (GotNumber) {
		return std::make_unique<NumberExpression>(Result);
	}
	throw Exception(Stream.str(), L"此处需要表达式");
}

std::unique_ptr<Expression> GetTerm(std::stringstream& Stream)
{
	try {
		return GetNumber(Stream);
	} catch (Exception&) {
		if (Is(Stream, '(')) {
			auto Result = GetExp(Stream);
			if (!Is(Stream, ')')) {
				throw Exception(Stream.str(), L"此处需要右括号");
			}
			return Result;
		}
		throw;
	}
}

std::unique_ptr<Expression> GetFactor(std::stringstream& Stream)
{
	auto Result = GetTerm(Stream);
	while (true) {
		BinaryOperator Operator;
		if (Is(Stream, '*')) { Operator = BinaryOperator::Multiplication; } else if (Is(Stream, '/')) { Operator = BinaryOperator::Division; } else { break; }
		Result = std::make_unique<BinaryExpression>(Operator, move(Result), GetTerm(Stream));
	}
	return Result;
}

std::unique_ptr<Expression> GetExp(std::stringstream& Stream)
{
	auto Result = GetFactor(Stream);
	while (true) {
		BinaryOperator Operator;
		if (Is(Stream, '+')) { Operator = BinaryOperator::Plus; } else if (Is(Stream, '-')) { Operator = BinaryOperator::Minus; } else { break; }
		Result = std::make_unique<BinaryExpression>(Operator, move(Result), GetFactor(Stream));
	}
	return Result;
}
			                
std::unique_ptr<Expression> GetExp(std::string Exp){
	std::stringstream ss(Exp);
	return GetExp(ss);
}

#endif