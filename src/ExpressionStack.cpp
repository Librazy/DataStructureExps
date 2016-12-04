#include "ExpressionStack.h"
#include "StrException.h"

#ifdef ExpressionStack_defined

const expression_stack::precede expression_stack::precede_table[7][7]{
	{ precede::H, precede::H, precede::L, precede::L, precede::L, precede::H, precede::H },
	{ precede::H, precede::H, precede::L, precede::L, precede::L, precede::H, precede::H },
	{ precede::H, precede::H, precede::H, precede::H, precede::L, precede::H, precede::H },
	{ precede::H, precede::H, precede::H, precede::H, precede::L, precede::H, precede::H },
	{ precede::L, precede::L, precede::L, precede::L, precede::L, precede::E, precede::F },
	{ precede::H, precede::H, precede::H, precede::H, precede::F, precede::H, precede::H },
	{ precede::L, precede::L, precede::L, precede::L, precede::L, precede::F, precede::E },
};

const std::map<char, expression_stack::op> expression_stack::operator_map{
	{ '+',op::Plus },
	{ '-',op::Minus },
	{ '*',op::Multiplication },
	{ '/',op::Division },
	{ '(',op::LBracket },
	{ ')',op::RBracket },
	{ '#',op::Sharp },
};

double expression_stack::eval(std::string exp)
{
	auto optr = std::stack<op>();
	auto opnd = std::stack<double>();

	optr.push(op::Sharp);

	auto ss = std::stringstream(exp + "#");
	do {
		if ('0' <= ss.peek() && ss.peek() <= '9') {
			int n;
			ss >> n;
			opnd.push(n);
		}
		else {
			auto n = operator_map.at(ss.peek());

		CheckPrecede:

			auto op = optr.top();
			double a, b;
			switch (precede_table[static_cast<size_t>(optr.top())][static_cast<size_t>(n)]) {

			case precede::L:
				optr.push(n); ss.get();
				break;

			case precede::E:
				optr.pop(); ss.get();
				break;

			case precede::H:

				optr.pop();
				if(opnd.size() <2) {
					goto Failed;
				}
				a = opnd.top();
				opnd.pop();
				b = opnd.top();
				opnd.pop();

				switch (op) {
				case op::Plus: opnd.push(b + a);
					goto CheckSharp;
				case op::Minus: opnd.push(b - a);
					goto CheckSharp;
				case op::Multiplication: opnd.push(b * a);
					goto CheckSharp;
				case op::Division: opnd.push(b / a);
					goto CheckSharp;

				Failed:
				case op::LBracket:
				case op::RBracket:
				case op::Sharp:
				default:
					throw str_exception(ss.str(), L"错误的操作符");
				}
			case precede::F:
			default:
				throw str_exception(ss.str(), L"错误的优先级");
			}
			continue;
			CheckSharp:
			if (optr.top() != op::Sharp) {
				goto CheckPrecede;
			}
		}
	} while (ss.peek() != '#' || optr.top() != op::Sharp);

	return opnd.top();
}

#endif