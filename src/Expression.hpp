#pragma once

#ifdef Expression_enabled

#ifndef Expression_defined

#include<memory>

enum class BinaryOperator
{
	Plus = '+',
	Minus = '-',
	Multiply = '*',
	Divide = '/',
};

struct Expression
{
	bool IsLeft;
	Expression();
	virtual ~Expression() = default;
	virtual double Eval() = 0;
};

struct NumberExpression : Expression
{
	int Value;

	explicit NumberExpression(int number);

	double Eval() override;
};

struct BinaryExpression : Expression
{
	std::unique_ptr<Expression> First;
	std::unique_ptr<Expression> Second;
	BinaryOperator Op;
	BinaryExpression(BinaryOperator theOp, std::unique_ptr<Expression>&& theLeft, std::unique_ptr<Expression>&& theRight);
	double Eval() override;
};

struct Exception
{
	const char* Start;
	const wchar_t* Error;

	Exception(const char* aStart, const wchar_t* aError);
};

bool Is(const char*& Stream, const char* Text);

std::unique_ptr<Expression> GetNumber(const char*& Stream);

std::unique_ptr<Expression> GetTerm(const char*& Stream);

std::unique_ptr<Expression> GetFactor(const char*& Stream);

std::unique_ptr<Expression> GetExp(const char*& Stream);

#define Expression_defined

#endif

#endif