#include "ExpressionStack.hpp"

#ifdef ExpressionStack_defined

constexpr ExpressionStack::Precede ExpressionStack::PrecedeTable[7][7]{
	{ Precede::H, Precede::H, Precede::L, Precede::L, Precede::L, Precede::H, Precede::H },
	{ Precede::H, Precede::H, Precede::L, Precede::L, Precede::L, Precede::H, Precede::H },
	{ Precede::H, Precede::H, Precede::H, Precede::H, Precede::L, Precede::H, Precede::H },
	{ Precede::H, Precede::H, Precede::H, Precede::H, Precede::L, Precede::H, Precede::H },
	{ Precede::L, Precede::L, Precede::L, Precede::L, Precede::L, Precede::E, Precede::F },
	{ Precede::H, Precede::H, Precede::H, Precede::H, Precede::F, Precede::H, Precede::H },
	{ Precede::L, Precede::L, Precede::L, Precede::L, Precede::L, Precede::F, Precede::E },
};

const std::map<char, ExpressionStack::Operator> ExpressionStack::OperatorMap{
	{ '+',Operator::Plus },
	{ '-',Operator::Minus },
	{ '*',Operator::Multiplication },
	{ '/',Operator::Division },
	{ '(',Operator::LBracket },
	{ ')',Operator::RBracket },
	{ '#',Operator::Sharp },
};

double ExpressionStack::Eval(std::string exp)
{
	auto optr = std::stack<Operator>();
	auto opnd = std::stack<double>();

	optr.push(Operator::Sharp);

	auto ss = std::stringstream(exp + "#");
	do {
		if ('0' <= ss.peek() && ss.peek() <= '9') {
			int n;
			ss >> n;
			opnd.push(n);
		}
		else {
			auto n = OperatorMap.at(ss.peek());

		CheckPrecede:

			auto op = optr.top();
			double a, b;
			switch (PrecedeTable[static_cast<size_t>(optr.top())][static_cast<size_t>(n)]) {

			case Precede::L:
				optr.push(n); ss.get();
				break;

			case Precede::E:
				optr.pop(); ss.get();
				break;

			case Precede::H:

				optr.pop();

				a = opnd.top();
				opnd.pop();
				b = opnd.top();
				opnd.pop();

				switch (op) {
				case Operator::Plus: opnd.push(b + a);
					goto CheckSharp;
				case Operator::Minus: opnd.push(b - a);
					goto CheckSharp;
				case Operator::Multiplication: opnd.push(b * a);
					goto CheckSharp;
				case Operator::Division: opnd.push(b / a);
					goto CheckSharp;
				case Operator::LBracket:
				case Operator::RBracket:
				case Operator::Sharp:
				default:
					throw std::runtime_error("Invaild operator");
				}

			case Precede::F:
			default:
				throw std::runtime_error("Invaild precede");
			}
			continue;
			CheckSharp:
			if (optr.top() != Operator::Sharp) {
				goto CheckPrecede;
			}
		}
	} while (ss.peek() != '#' || optr.top() != Operator::Sharp);

	return opnd.top();
}

#endif