#pragma once

#ifdef Expression_enabled

#ifndef Expression_defined

#include<memory>

/**
 * \brief ��Ԫ������
 */
enum class BinaryOperator
{
	Plus = '+',
	Minus = '-',
	Multiply = '*',
	Divide = '/',
};

/**
 * \brief ������ʽ
 */
struct Expression
{
	bool IsLeft;

	Expression();
	virtual ~Expression() = default;
	virtual double Eval() = 0;
};

/**
 * \brief ����ֵ���ʽ
 */
struct NumberExpression : Expression
{
	int Value;

	explicit NumberExpression(int number);

	double Eval() override;
};

/**
 * \brief ��Ԫ������ʽ
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
 * \brief �쳣��
 */
struct Exception
{
	const char* Start;
	const wchar_t* Error;

	Exception(const char* aStart, const wchar_t* aError);
};

/**
 * \brief ���ʽ�� @Stream ����һ���Ƿ��� @Text
 * \param Stream ���ʽ��
 * \param Text ��Ҫ�жϵ��ַ�
 * \return �Ƿ�Ϊ @Text
 */
bool Is(const char*& Stream, const char* Text);

/**
 * \brief �ڱ��ʽ������ȡ��ֵ
 * \param Stream ���ʽ��
 * \return ָ���ȡ��NumberExpression��unique_ptr
 */
std::unique_ptr<NumberExpression> GetNumber(const char*& Stream);

/**
 * \brief �ڱ��ʽ������ȡTerm: Number | ��(�� Exp ��)��
 * \param Stream ���ʽ��
 * \return ָ���ȡ��Expression��unique_ptr
 */
std::unique_ptr<Expression> GetTerm(const char*& Stream);

/**
 * \brief �ڱ��ʽ������ȡFacto: Term ( ( ��*�� | ��/�� ) Term) *
 * \param Stream ���ʽ��
 * \return ָ���ȡ��Expression��unique_ptr
 */
std::unique_ptr<Expression> GetFactor(const char*& Stream);

/**
 * \brief �ڱ��ʽ������ȡExp: Factor ( ( ��+�� | ��-�� ) Factor) *
 * \param Stream ���ʽ��
 * \return ָ���ȡ��Expression��unique_ptr
 */
std::unique_ptr<Expression> GetExp(const char*& Stream);

#define Expression_defined

#endif

#endif