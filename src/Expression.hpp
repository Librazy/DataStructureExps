#pragma once

#ifdef Expression_enabled

#ifndef Expression_defined

#include<memory>

/**
 * \brief 二元操作符
 */
enum class BinaryOperator
{
	Plus = '+',
	Minus = '-',
	Multiply = '*',
	Divide = '/',
};

/**
 * \brief 抽象表达式
 */
struct Expression
{
	bool IsLeft;

	Expression();
	virtual ~Expression() = default;
	virtual double Eval() = 0;
};

/**
 * \brief 单数值表达式
 */
struct NumberExpression : Expression
{
	int Value;

	explicit NumberExpression(int number);

	double Eval() override;
};

/**
 * \brief 二元运算表达式
 */
struct BinaryExpression : Expression
{
	std::unique_ptr<Expression> First;
	std::unique_ptr<Expression> Second;
	BinaryOperator Op;

	BinaryExpression(BinaryOperator theOp, std::unique_ptr<Expression>&& theLeft, std::unique_ptr<Expression>&& theRight);
	double Eval() override;
};

/**
 * \brief 异常类
 */
struct Exception
{
	const char* Start;
	const wchar_t* Error;

	Exception(const char* aStart, const wchar_t* aError);
};

/**
 * \brief 表达式流 @Stream 的下一个是否是 @Text
 * \param Stream 表达式流
 * \param Text 需要判断的字符
 * \return 是否为 @Text
 */
bool Is(const char*& Stream, const char* Text);

/**
 * \brief 在表达式流中提取数值
 * \param Stream 表达式流
 * \return 指向获取的NumberExpression的unique_ptr
 */
std::unique_ptr<NumberExpression> GetNumber(const char*& Stream);

/**
 * \brief 在表达式流中提取Term: Number | “(“ Exp “)”
 * \param Stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<Expression> GetTerm(const char*& Stream);

/**
 * \brief 在表达式流中提取Facto: Term ( ( “*” | “/” ) Term) *
 * \param Stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<Expression> GetFactor(const char*& Stream);

/**
 * \brief 在表达式流中提取Exp: Factor ( ( “+” | “-“ ) Factor) *
 * \param Stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<Expression> GetExp(const char*& Stream);

#define Expression_defined

#endif

#endif