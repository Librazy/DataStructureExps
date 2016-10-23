#include "Expression.hpp"

#ifdef Expression_defined

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

BinaryExpression::BinaryExpression(BinaryOperator theOp, std::unique_ptr<Expression>&& theLeft, std::unique_ptr<Expression>&& theRight) :
	First(move(theLeft)), Second(move(theRight)), Op(theOp)
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
	case BinaryOperator::Multiply:
		return lhs * rhs;
	case BinaryOperator::Divide:
		return lhs / rhs;
	default: throw Exception(nullptr, nullptr);
	}
}

Exception::Exception(const char* aStart, const wchar_t* aError)
{
	Start = aStart;
	Error = aError;
}

bool Is(const char*& Stream, const char* Text)
{
	auto len = strlen(Text);
	auto Read = Stream;
	while (*Read == ' ')Read++;
	if (strncmp(Read, Text, len) == 0) {
		Stream = Read + len;
		return true;
	}
	return false;
}

std::unique_ptr<Expression> GetNumber(const char*& Stream)
{
	auto Result = 0;
	auto GotNumber = false;
	auto Read = Stream;
	while (*Read == ' ')Read++;
	while (true) {
		auto c = *Read;
		if ('0' <= c && c <= '9') {
			Result = Result * 10 + (c - '0');
			GotNumber = true;
			Read++;
		} else { break; }
	}
	if (GotNumber) {
		Stream = Read;
		return std::make_unique<NumberExpression>(Result);
	}
	throw Exception(Stream, L"此处需要表达式");
}

std::unique_ptr<Expression> GetTerm(const char*& Stream)
{
	try {
		return GetNumber(Stream);
	} catch (Exception) {
		auto Read = Stream;
		if (Is(Read, "(")) {
			auto Result = GetExp(Read);
			if (Is(Read, ")")) {
				Stream = Read;
				return Result;
			}
			throw Exception(Stream, L"此处需要右括号");
		}
		throw;
	}
}

std::unique_ptr<Expression> GetFactor(const char*& Stream)
{
	auto Read = Stream;
	auto Result = GetTerm(Read);
	while (true) {
		BinaryOperator Operator;
		if (Is(Read, "*")) { Operator = BinaryOperator::Multiply; } else if (Is(Read, "/")) { Operator = BinaryOperator::Divide; } else { break; }
		Result = std::make_unique<BinaryExpression>(Operator, move(Result), GetTerm(Read));
	}
	Stream = Read;
	return Result;
}

std::unique_ptr<Expression> GetExp(const char*& Stream)
{
	auto Read = Stream;
	auto Result = GetFactor(Read);
	while (true) {
		BinaryOperator Operator;
		if (Is(Read, "+")) { Operator = BinaryOperator::Plus; } else if (Is(Read, "-")) { Operator = BinaryOperator::Minus; } else { break; }
		Result = std::make_unique<BinaryExpression>(Operator, move(Result), GetFactor(Read));
	}
	Stream = Read;
	return Result;
}

#endif