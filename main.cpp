// ReSharper disable CppUnusedIncludeDirective
#undef NDEBUG
#include "src/Exception.hpp"
#include "src/ExpressionTree.hpp"
#include "src/ExpressionStack.hpp"
#include "src/SparseMatrix.hpp"
#include "src/BFS.hpp"
#include "src/BinaryTree.hpp"

#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <stdexcept>

int main()
{
#ifdef Use_Wcout

	try {
#ifdef _WIN32
		std::wcout.imbue(std::locale("chs"));
#else //_WIN32
		std::wcout.imbue(std::locale("zh_CN.UTF-8"));
#endif //_WIN32
	}
	catch (std::runtime_error) {
		setlocale(LC_ALL, "chs");
		std::ios_base::sync_with_stdio(false);
	}
#endif //Use_Wcout

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

		assert((mat5.get(1, 1) == 0));
		assert((mat5.get<1, 1>() == 0));

		try{
			mat5.get(3, 5);
			throw std::runtime_error("Exception Expected");
		}catch(std::out_of_range e){
			assert(std::string(e.what()) == "Matrix bound check failed");
		}

		auto mat6 = mat - mat5;
		assert((mat6.get<1, 2>() == 2));
		std::stringstream ss;
		ss << mat;
		assert(ss.str() == "1 1 0\n0 0 4\n");

	}
#ifdef Use_Wcout
	std::wcout << L"SparseMatrix2 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "SparseMatrix2 test complete" << std::endl;
#endif //Use_Wcout
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
		ss1 << '\n' << t;
		auto anstext =
R"(
(0,0)
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
		ss3 << '\n' << w;
		auto ans =
R"(
FW   
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
#ifdef Use_Wcout
	std::wcout << L"BFS 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "BFS test complete" << std::endl;
#endif //Use_Wcout
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
#ifdef Use_Wcout
	std::wcout << L"ExpressionStack 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "ExpressionStack test complete" << std::endl;
#endif //Use_Wcout
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
		}catch(Exception& e){
			assert(std::wstring(e.Error) == L"此处需要右括号");
		}

		try{
			auto str3 = "2-*3";
			GetExp(str3);
			throw std::runtime_error("Exception Expected");
		}catch(Exception& e){
			assert(std::wstring(e.Error) == L"此处需要表达式");
		}

		try{
			auto brokenExp = BinaryExpression(static_cast<BinaryOperator>(1),std::make_unique<NumberExpression>(1),std::make_unique<NumberExpression>(1));
			brokenExp.Eval();
			throw std::runtime_error("Exception Expected");
		}catch(Exception& e){
			assert(std::wstring(e.Error) == L"错误的操作符");
		}
	}
#ifdef Use_Wcout
	std::wcout << L"ExpressionTree 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "ExpressionTree test complete" << std::endl;
#endif //Use_Wcout
	//++End ExpressionTree test
#endif

#ifndef BinaryTree_disabled
	//++Start BinaryTree test
	{
/*
 *      1
 *     / \
 *    2   3
 *   / \
 *  4   5
 */

		auto *p4 = new int(4), t4 = 4, t5 = 5, t2 = 2, t3 = 3, t1 = 1;
		auto tree = MakeTree(MakeTree(MakeTree<std::unique_ptr>(std::unique_ptr<int>(p4)), MakeTree<std::unique_ptr>(std::make_unique<int>(5)), std::make_unique<int>(2)), MakeTree<std::unique_ptr>(std::make_unique<int>(3)), std::make_unique<int>(1));
		auto tree2 = MakeTree(MakeTree(MakeTree(t4), MakeTree(t5), t2), MakeTree(t3), t1);
		auto ans =
R"(
1 2 4 5 3 
4 2 5 1 3 
4 5 2 3 1 
)";
		auto ans2 =
R"(
1 2 4 5 3 
4 2 5 1 3 
1 1 1 1 1 
)";
		auto ans3 =
R"(
1 2 4 5 3 
4 2 5 1 3 
5 6 3 4 2 
)";
		std::stringstream ss1, ss2, ss3, ssa(ans), ssa2(ans2), ssa3(ans3);

		ss1 << std::endl;
		tree->TreeTraversalRecursive<Order::PreOrder>([&ss1](auto& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		tree->TreeTraversalRecursive<Order::InOrder>([&ss1](std::unique_ptr<int>& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		tree->TreeTraversalRecursive<Order::PostOrder>([&ss1](auto& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		assert(ssa.str() == ss1.str());

		ss2 << std::endl;
		tree2->TreeTraversalRecursive<Order::PreOrder>([&ss2](int i) {ss2 << i << " "; });
		ss2 << std::endl;
		tree2->TreeTraversalRecursive<Order::InOrder>([&ss2](auto& i) {ss2 << i << " "; i = 1;});
		ss2 << std::endl;
		tree2->TreeTraversalRecursive<Order::PostOrder>([&ss2](auto i) {ss2 << i << " "; });
		ss2 << std::endl;
		assert(ssa2.str() == ss2.str());
		assert((t4|t5|t2|t3|t1) == 1);

		ss3 << std::endl;
		tree->TreeTraversalIterative<Order::PreOrder>([&ss3](auto& i) {ss3 << *i << " "; });
		ss3 << std::endl;
		tree->TreeTraversalIterative<Order::InOrder>([&ss3](auto& i) {ss3 << *i << " "; *i += 1; });
		ss3 << std::endl;
		tree->TreeTraversalIterative<Order::PostOrder>([&ss3](auto& i) {ss3 << *i << " "; });
		ss3 << std::endl;
		assert(ssa3.str() == ss3.str());
		assert(*p4 == 5);

		struct func
		{
			func(){}
			func(const func&) = delete;
			func(func&&) = delete;
			func& operator= (const func&) = delete;
			func& operator= (func&&) = delete;

			int sum = 0;
			void operator()(std::unique_ptr<int>& i){
				sum += *i;
			}
			void operator()(int& i) {
				sum += i;
			}
		};
		func f;
		tree->TreeTraversalRecursive<Order::PostOrder>(f);
		tree2->TreeTraversalRecursive<Order::InOrder>(f);
		assert(f.sum == 5 + 6 + 3 + 4 + 2 + 5);
	}
#ifdef Use_Wcout
	std::wcout << L"BinaryTree 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "BinaryTree test complete" << std::endl;
#endif //Use_Wcout
	//++End BinaryTree test
#endif
	return 0;
}
