#include "ExpressionTree.h"
#include "StrException.h"

#ifdef ExpressionTree_defined

expression::expression()
{
	is_left = false;
}

number_expression::number_expression(int number)
{
	value = number;
}

double number_expression::eval()
{
	return value;
}

binary_expression::binary_expression(bin_op op, std::unique_ptr<expression>&& first, std::unique_ptr<expression>&& second) :
	first(move(first)), second(move(second)), op(op)
{ }

double binary_expression::eval()
{
	auto lhs = first->eval();
	auto rhs = second->eval();
	
	switch (op) {
	case bin_op::Plus:
		return lhs + rhs;
	case bin_op::Minus:
		return lhs - rhs;
	case bin_op::Multiplication:
		return lhs * rhs;
	case bin_op::Division:
		return lhs / rhs;
	default: throw str_exception("", L"错误的操作符");
	}
}

bool is(std::stringstream& stream, const char text)
{
	auto read = stream.tellg();
	while (stream.peek() == ' ')stream.get();
	
	if (stream.peek() == text) {
		stream.get();
		return true;
	}
	stream.seekg(read);
	return false;
}

std::unique_ptr<number_expression> get_number(std::stringstream& stream)
{
	auto result = 0;
	auto got_number = false;
	while (stream.peek() == ' ')stream.get();
	while (true) {
		auto c = stream.peek();
		if ('0' <= c && c <= '9') {
			result = result * 10 + (c - '0');
			got_number = true;
			stream.get();
		} else { break; }
	}
	if (got_number) {
		return std::make_unique<number_expression>(result);
	}
	throw str_exception(stream.str(), L"此处需要表达式");
}

std::unique_ptr<expression> get_term(std::stringstream& stream)
{
	try {
		return get_number(stream);
	} catch (str_exception&) {
		if (is(stream, '(')) {
			auto result = get_exp(stream);
			if (!is(stream, ')')) {
				throw str_exception(stream.str(), L"此处需要右括号");
			}
			return result;
		}
		throw;
	}
}

std::unique_ptr<expression> get_factor(std::stringstream& stream)
{
	auto result = get_term(stream);
	while (true) {
		bin_op op;
		if (is(stream, '*')) { op = bin_op::Multiplication; } else if (is(stream, '/')) { op = bin_op::Division; } else { break; }
		result = std::make_unique<binary_expression>(op, move(result), get_term(stream));
	}
	return result;
}

std::unique_ptr<expression> get_exp(std::stringstream& stream)
{
	auto Result = get_factor(stream);
	while (true) {
		bin_op op;
		if (is(stream, '+')) { op = bin_op::Plus; } else if (is(stream, '-')) { op = bin_op::Minus; } else { break; }
		Result = std::make_unique<binary_expression>(op, move(Result), get_factor(stream));
	}
	return Result;
}
			                
std::unique_ptr<expression> get_exp(std::string exp){
	std::stringstream ss(exp);
	return get_exp(ss);
}

#endif