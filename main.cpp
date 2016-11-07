// ReSharper disable CppUnusedIncludeDirective
#include "src/Exception.hpp"
#include "src/ExpressionTree.hpp"
#include "src/ExpressionStack.hpp"
#include "src/SparseMatrix.hpp"
#include "src/BFS.hpp"

#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <stdexcept>
#undef NDEBUG

int main()
{
	try {
#ifdef _WIN32
		std::wcout.imbue(std::locale("chs"));
#else
		std::wcout.imbue(std::locale("zh_CN.UTF-8"));
#endif
	}
	catch (std::runtime_error) {
		setlocale(LC_ALL, "chs");
		std::ios_base::sync_with_stdio(false);
	}

#ifdef SparseMatrix_enabled
	//++Start SparseMatrix2 test
	{
		//auto mat0 = SparseMatrix2<int, 2, 3>({ { 1,1,0,4 },{ 0,0,4,4 } }); //将会触发编译器报错：Col size doesn't match

		auto mat = SparseMatrix2<int, 2, 3>({ { 1,1,0 },{0,0,4 } });

		auto mat2 = SparseMatrix2<int, 3, 1>();
		mat2.set<0, 0>(4);
		mat2.set<1, 0>(1);
		mat2.set<2, 0>(2);


		auto mat3 = mat * mat2;
		auto mat4 = mat3.Rev();

		assert((mat3.get<0, 0>() == 14));
		assert((mat3.get<1, 0>() == 28));
		assert((mat4.get<0, 0>() == 14));
		assert((mat4.get<0, 1>() == 28));
		//assert((mat4.get<0, 2>() == 28)); //将会触发编译器报错：Matrix bound check failed

		auto mat5 = SparseMatrix2<int, 2, 3>();
		mat5.set<0, 0>(1);
		mat5.set<0, 1>(1);
		mat5.set<1, 2>(2);

		auto mat6 = mat - mat5;
		assert((mat6.get<1, 2>() == 2));
		std::stringstream ss;
		ss << mat;
		assert(ss.str() == "1 1 0\n0 0 4\n");

	}
	std::wcout << L"SparseMatrix2 测试完成" << std::endl;
	//++End SparseMatrix2 test
#endif

#ifndef BFS_disabled
	//++Start BFS test
	{
		auto map =
R"(
0 1 0 0 0
0 1 0 1 0
0 0 0 0 0
0 1 1 1 0
0 0 0 1 0
)";
		auto x = BFS(0, 0, 4, 4, 5, 5, map);

		auto t = BFS_pretty_text(x);
		std::stringstream ss1, ss2;
		ss1 << t;
		auto anstext =
R"((0,0)
(1,0)
(2,0)
(2,1)
(2,2)
(2,3)
(2,4)
(3,4)
(4,4)
)";
		ss2 << anstext;
		assert(ss1.str() == ss2.str());

		auto w = BFS_pretty_graph(x, 5, 5, map);
		std::stringstream ss3, ss4;
		ss3 << w;
		auto ans =
R"(FW   
DW W 
RRRRD
 WWWD
   WF
)";
		ss4 << ans;
		assert(ss3.str() == ss4.str());

		auto map2 =
R"(
0 1 0 
0 1 0 
)";
		auto x2 = BFS(0, 0, 2, 1, 3, 2, map2);
		assert(x2.size() == 0);
	}
	std::wcout << L"BFS 测试完成" << std::endl;
	//++End BFS test
#endif

#ifndef ExpressionStack_disabled
	//++Start ExpressionStack test
	{
		auto str = "1+2*3";
		auto ans = ExpressionStack::Eval(str);
		assert(ans == 7.0);

		auto str1 = "2-3*4-5/6";
		auto ans1 = ExpressionStack::Eval(str1);
		assert(fabs(ans1 - (2 - 3 * 4 - 5.0 / 6)) < 0.01);

		auto str2 = "(1-2)*3";
		auto ans2 = ExpressionStack::Eval(str2);
		assert(ans2 == -3.0);

		auto str3 = "1+2+(40*34-22)*2";
		auto ans3 = ExpressionStack::Eval(str3);
		assert(ans3 == 2679.0);

		auto str4 = "2-3*4-5/6+7*(8-(9*10+1)/2+20+2*10)-20+12";
		auto ans4 = ExpressionStack::Eval(str4);
		assert(fabs(ans4 - (2 - 3 * 4 - 5.0 / 6 + 7 * (8 - (9 * 10 + 1.0) / 2 + 20 + 2 * 10) - 20 + 12)) < 0.01);
		try {
			ExpressionStack::Eval("3+4)4");
			throw std::runtime_error("Exception Expected");
		}
		catch (Exception& e) {
			assert(std::wstring(e.Error) == L"错误的优先级");
		}

		try {
			ExpressionStack::Eval("*5-4");
			throw std::runtime_error("Exception Expected");
		}
		catch (Exception& e) {
			assert(std::wstring(e.Error) == L"错误的操作符");
		}
	}
	std::wcout << L"ExpressionStack 测试完成" << std::endl;
	//++End ExpressionStack test
#endif


#ifndef ExpressionTree_disabled
	//++Start ExpressionTree test
	{
		auto str = "1+2+   (40*34-22)*2";
		auto exp = GetExp(str);
		assert(exp->Eval() == 2679.0);

		auto str1 = "2-3*4-5/6";
		auto exp1 = GetExp(str1);
		assert(fabs(exp1->Eval() - (2 - 3 * 4 - 5.0 / 6)) < 0.01);
		
		auto str2 = "2-3*4-5/6 + 7*(8-   (9*10+1)/2 +20+2*10    ) -20 +12";
		auto exp2 = GetExp(str2);
		assert(fabs(exp2->Eval() - (2 - 3 * 4 - 5.0 / 6 + 7 * (8 - (9 * 10 + 1.0) / 2 + 20 + 2 * 10) - 20 + 12)) < 0.01);
		
		try{
			auto str3 = "2-(3";
			GetExp(str3);
			throw std::runtime_error("Exception Expected");
		}catch(Exception e){
			assert(std::wstring(e.Error) == L"此处需要右括号");
		}

		try{
			auto str3 = "2-*3";
			GetExp(str3);
			throw std::runtime_error("Exception Expected");
		}catch(Exception e){
			assert(std::wstring(e.Error) == L"此处需要表达式");
		}
		
		try{
			auto brokenExp = BinaryExpression(static_cast<BinaryOperator>(1),std::make_unique<NumberExpression>(1),std::make_unique<NumberExpression>(1));
			brokenExp.Eval();
			throw std::runtime_error("Exception Expected");
		}catch(Exception e){
			assert(std::wstring(e.Error) == L"错误的操作符");
		}
	}
	std::wcout << L"ExpressionTree 测试完成" << std::endl;
	//++End ExpressionTree test
#endif
	return 0;
}
