#pragma once

#ifndef ExpressionTree_disabled

#ifndef ExpressionTree_defined

// ReSharper disable CppUnusedIncludeDirective
#include<memory>
#include<cstring>
#include<sstream>

/**
 * \brief 二元操作符
 */
enum class bin_op
{
	Plus           = '+',
	Minus          = '-',
	Multiplication = '*',
	Division       = '/',
};

/**
 * \brief 抽象表达式
 */
struct expression
{
	bool is_left;

	expression();
	virtual ~expression() = default;
	virtual double eval() = 0;
};

/**
 * \brief 单数值表达式
 */
struct number_expression : expression
{
	int value;

	explicit number_expression(int number);
	double eval() override;
};

/**
 * \brief 二元运算表达式
 */
struct binary_expression : expression
{
	std::unique_ptr<expression> first;
	std::unique_ptr<expression> second;
	bin_op op;

	binary_expression(bin_op op, std::unique_ptr<expression>&& first, std::unique_ptr<expression>&& second);
	double eval() override;
};

/**
 * \brief 表达式流 @Stream 的下一个是否是 @text
 * \param stream 表达式流
 * \param text 需要判断的字符
 * \return 是否为 @text
 */
bool is(std::stringstream& stream, const char text);

/**
 * \brief 在表达式流中提取数值
 * \param stream 表达式流
 * \return 指向获取的NumberExpression的unique_ptr
 */
std::unique_ptr<number_expression> get_number(std::stringstream& stream);

/**
 * \brief 在表达式流中提取Term: Number | "(" Exp ")"
 * \param stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<expression> get_term(std::stringstream& stream);

/**
 * \brief 在表达式流中提取Facto: Term ( ( "*" | "/" ) Term) *
 * \param stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<expression> get_factor(std::stringstream& stream);

/**
 * \brief 在表达式流中提取Exp: Factor ( ( "+" | "-" ) Factor) *
 * \param stream 表达式流
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<expression> get_exp(std::stringstream& stream);

/**
 * \brief 在表达式中提取Exp: Factor ( ( "+" | "-" ) Factor) *
 * \param exp 表达式
 * \return 指向获取的Expression的unique_ptr
 */
std::unique_ptr<expression> get_exp(std::string exp);

#define ExpressionTree_defined

#endif

#endif