// ReSharper disable CppUnusedIncludeDirective
#undef NDEBUG
#include "src/StrException.h"
#include "src/ExpressionTree.h"
#include "src/ExpressionStack.h"
#include "src/SparseMatrix.hpp"
#include "src/BFS.h"
#include "src/BinaryTree.hpp"
#include "src/Dijkstra.h"
#include "src/Kruskal.h"
#include "src/AVL.hpp"
#include "main.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <set>
#include <numeric>
#include <algorithm>
#include <random>

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

#ifndef SparseMatrix_disabled
	//++Start SparseMatrix2 test
	{
#ifdef Use_FoldExp
		static_assert(!dim_bound_check_static<1, 2>(1, 2), "static_assert failed to detect error");
		static_assert(!dim_bound_check_static<1, 2, 3>(0, 1, 3), "static_assert failed to detect error");
		static_assert(!dim_bound_check_static<4, 1, 1>(3, 1, 0), "static_assert failed to detect error");
		static_assert(dim_bound_check_static<1, 1>(0, 0), "static_assert false alarm");
		static_assert(dim_bound_check_static<4, 2>(3, 0), "static_assert false alarm");
		static_assert(dim_bound_check_static<4, 7>(0, 6), "static_assert false alarm");
#else
		static_assert(!dim_bound_check_static<1, 2>(1, 2), "static_assert failed to detect error");
		static_assert(!dim_bound_check_static<4, 2>(1, 2), "static_assert failed to detect error");
		static_assert(dim_bound_check_static<1, 1>(0, 0), "static_assert false alarm");
		static_assert(dim_bound_check_static<4, 2>(3, 0), "static_assert false alarm");
		static_assert(dim_bound_check_static<4, 7>(0, 6), "static_assert false alarm");
#endif

		//auto mat0 = sparse_matrix2d<int, 2, 3>({ { 1,1,0,4 },{ 0,0,4,4 } }); //将会触发编译器报错：Col size doesn't match
		int out;

		auto mat = sparse_matrix2d<int, 2, 3>({ { 1,1,0 },{ 0,0,4 } });
		assert((mat.get<0, 2>() == 0));
		assert(mat.get(0, 2) == 0);
		assert(mat.have(0, 0, out));
		assert((!mat.have<0, 2>(out)));

		auto mat2 = sparse_matrix2d<int, 3, 1>();
		mat2.set<0, 0>(4);
		mat2.set<1, 0>(1);
		mat2.set<2, 0>(2);

		auto mat3 = mat * mat2;
		auto mat4 = mat3.Rev();

		assert((mat3.have<0, 0>(out) && out == 14));
		assert((mat3.get<1, 0>() == 28));
		assert((mat4.get<0, 0>() == 14));
		assert((mat4.get<0, 1>() == 28));
		//assert((mat4.get<0, 2>() == 28)); //将会触发编译器报错：Matrix bound check failed

		auto mat5 = sparse_matrix2d<int, 2, 3>();
		mat5.set(1, 0, 0);
		mat5.set<0, 1>(1);
		mat5.set<1, 2>(2);

		assert(!mat5.have(1, 1, out) && out == 0);
		assert(mat5.get(1, 1) == 0);

		auto m5r0 = mat5.row(0);
		assert(m5r0.size() == 2);
		assert(m5r0[1].second == 1);

		auto m5r1 = mat5.row<1>();
		assert(m5r1.size() == 1);
		assert(m5r1[0].second == 2);

		try {
			mat5.row(4);
			throw std::runtime_error("std::out_of_range expected");
		}
		catch (std::out_of_range e) {
			assert(std::string(e.what()) == "Matrix bound check failed");
		}

		try{
			mat5.get(3, 5);
			throw std::runtime_error("std::out_of_range expected");
		}catch(std::out_of_range e){
			assert(std::string(e.what()) == "Matrix bound check failed");
		}

		auto mat6 = mat - mat5;
		assert((mat6.get<1, 2>() == 2));
		std::stringstream ss;
		ss << mat;
		assert(ss.str() == "1 1 0\n0 0 4\n");

		auto mat7 = sparse_matrix2d<int, 2, 3>({ { 0,0,0 },{ 0,0,0 } });
		assert(mat7.row(0).size() == 0);
		assert((mat7.row<1>().size() == 0));
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
		auto x = bfs(0, 0, 4, 4, 5, 5, map);

		auto t = bfs_pretty_text(x);
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

		auto w = bfs_pretty_graph(x, 5, 5, map);
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
		auto x2 = bfs(0, 0, 2, 1, 3, 2, map2);
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
		auto ans = expression_stack::eval(str);
		assert(ans == 7.0);

		auto str1 = "2-3*4-5/6";
		auto ans1 = expression_stack::eval(str1);
		assert(fabs(ans1 - (2 - 3 * 4 - 5.0 / 6)) < 0.01);

		auto str2 = "(1-2)*3";
		auto ans2 = expression_stack::eval(str2);
		assert(ans2 == -3.0);

		auto str3 = "1+2+(40*34-22)*2";
		auto ans3 = expression_stack::eval(str3);
		assert(ans3 == 2679.0);

		auto str4 = "2-3*4-5/6+7*(8-(9*10+1)/2+20+2*10)-20+12";
		auto ans4 = expression_stack::eval(str4);
		assert(fabs(ans4 - (2 - 3 * 4 - 5.0 / 6 + 7 * (8 - (9 * 10 + 1.0) / 2 + 20 + 2 * 10) - 20 + 12)) < 0.01);
		try {
			expression_stack::eval("3+4)4");
			throw std::runtime_error("str_exception expected");
		}
		catch (str_exception& e) {
			assert(std::wstring(e.error) == L"错误的优先级");
		}

		try {
			expression_stack::eval("*5-4");
			throw std::runtime_error("str_exception expected");
		}
		catch (str_exception& e) {
			assert(std::wstring(e.error) == L"错误的操作符");
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
		auto exp = get_exp(str);
		assert(exp->eval() == 2679.0);

		auto str1 = "2-3*4-5/6";
		auto exp1 = get_exp(str1);
		assert(fabs(exp1->eval() - (2 - 3 * 4 - 5.0 / 6)) < 0.01);

		auto str2 = "2-3*4-5/6 + 7*(8-   (9*10+1)/2 +20+2*10    ) -20 +12";
		auto exp2 = get_exp(str2);
		assert(fabs(exp2->eval() - (2 - 3 * 4 - 5.0 / 6 + 7 * (8 - (9 * 10 + 1.0) / 2 + 20 + 2 * 10) - 20 + 12)) < 0.01);

		try{
			auto str3 = "2-(3";
			get_exp(str3);
			throw std::runtime_error("str_exception expected");
		}catch(str_exception& e){
			assert(std::wstring(e.error) == L"此处需要右括号");
		}

		try{
			auto str3 = "2-*3";
			get_exp(str3);
			throw std::runtime_error("str_exception expected");
		}catch(str_exception& e){
			assert(std::wstring(e.error) == L"此处需要表达式");
		}

		try{
			auto brokenExp = binary_expression(static_cast<bin_op>(1),std::make_unique<number_expression>(1),std::make_unique<number_expression>(1));
			brokenExp.eval();
			throw std::runtime_error("str_exception expected");
		}catch(str_exception& e){
			assert(std::wstring(e.error) == L"错误的操作符");
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
		auto tree = make_tree(make_tree(make_tree<std::unique_ptr>(std::unique_ptr<int>(p4)), make_tree<std::unique_ptr>(std::make_unique<int>(5)), std::make_unique<int>(2)), make_tree<std::unique_ptr>(std::make_unique<int>(3)), std::make_unique<int>(1));
		auto tree2 = make_tree(make_tree(make_tree(t4), make_tree(t5), t2), make_tree(t3), t1);
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
		tree->traversal_recursive<Order::PreOrder>([&ss1](auto& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		tree->traversal_recursive<Order::InOrder>([&ss1](std::unique_ptr<int>& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		tree->traversal_recursive<Order::PostOrder>([&ss1](auto& i) {ss1 << *i << " "; });
		ss1 << std::endl;
		assert(ssa.str() == ss1.str());

		ss2 << std::endl;
		tree2->traversal_recursive<Order::PreOrder>([&ss2](int i) {ss2 << i << " "; });
		ss2 << std::endl;
		tree2->traversal_recursive<Order::InOrder>([&ss2](auto& i) {ss2 << i << " "; i = 1;});
		ss2 << std::endl;
		tree2->traversal_recursive<Order::PostOrder>([&ss2](auto i) {ss2 << i << " "; });
		ss2 << std::endl;
		assert(ssa2.str() == ss2.str());
		assert((t4|t5|t2|t3|t1) == 1);

		ss3 << std::endl;
		tree->traversal_iterative<Order::PreOrder>([&ss3](auto& i) {ss3 << *i << " "; });
		ss3 << std::endl;
		tree->traversal_iterative<Order::InOrder>([&ss3](auto& i) {ss3 << *i << " "; *i += 1; });
		ss3 << std::endl;
		tree->traversal_iterative<Order::PostOrder>([&ss3](auto& i) {ss3 << *i << " "; });
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
		tree->traversal_recursive<Order::PostOrder>(f);
		tree2->traversal_recursive<Order::InOrder>(f);
		assert(f.sum == 5 + 6 + 3 + 4 + 2 + 5);
	}
#ifdef Use_Wcout
	std::wcout << L"BinaryTree 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "BinaryTree test complete" << std::endl;
#endif //Use_Wcout
	//++End BinaryTree test
#endif

#ifndef Dijkstra_disabled
	//++Start Dijkstra test
	{
		auto map = sparse_matrix2d<int, 6, 6>({
			{ 0, 2, 3, 0, 0, 0 },
			{ 2, 0, 0, 4, 2, 0 },
			{ 3, 0, 0, 2, 2, 7 },
			{ 0, 4, 2, 0, 0, 3 },
			{ 0, 2, 2, 0, 0, 4 },
			{ 0, 0, 7, 3, 4, 0 },
		});
		auto ret = dijkstra(map, 0);
		auto ans = std::array<int, 6>{ {0, 2, 3, 5, 4, 8} };
		assert(ret == ans);
	}
#ifdef Use_Wcout
	std::wcout << L"Dijkstra 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "Dijkstra test complete" << std::endl;
#endif //Use_Wcout
	//++End Dijkstra test
#endif

#ifndef Kruskal_disabled
//++Start Kruskal test
	{
		auto map = sparse_matrix2d<int, 6, 6>({
			{ 0, 2, 3, 0, 0, 0 },
			{ 2, 0, 0, 4, 2, 0 },
			{ 3, 0, 0, 2, 2, 7 },
			{ 0, 4, 2, 0, 0, 3 },
			{ 0, 2, 2, 0, 0, 4 },
			{ 0, 0, 7, 3, 4, 0 },
		});
		auto ret = kruskal(map);
		assert(ret.size() == 5);
		auto total = std::accumulate(ret.begin(), ret.end(), 0, [](auto i, auto j) { return i + std::get<0>(j); });
		assert(total == 2 + 2 + 2 + 2 + 3);
		auto s = std::set<size_t>();
		std::for_each(ret.begin(), ret.end(), [&s](auto i)
		{
			s.insert(std::get<1>(i));
			s.insert(std::get<2>(i));
		});
		assert(s.size() == 6);
	}
#ifdef Use_Wcout
	std::wcout << L"Kruskal 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "Kruskal test complete" << std::endl;
#endif //Use_Wcout
	//++End Kruskal test
#endif

#ifndef AVL_disabled
//++Start AVL test
	{
		std::stringstream ss1, ss2, ss3, ss4;

		auto ans =
			R"(
4 2 0 -1 1 3 5 6 
-1 0 1 2 3 4 5 6 
)";

		auto tree = avl_tree<int>();
		tree.insert(3);
		tree.insert(4);
		tree.insert(5);
		tree.insert(6);
		tree.insert(2);
		tree.insert(1);
		tree.insert(0);
		tree.insert(-1);

		ss1 << std::endl;
		tree.traversal_recursive<Order::PreOrder>([&ss1](auto& a)
		{
			ss1 << a << " ";
		});
		ss1 << std::endl;
		tree.traversal_recursive<Order::InOrder>([&ss1](auto& a)
		{
			ss1 << a << " ";
		});
		ss1 << std::endl;
		auto i = tree.search(3);
		assert(i == 3);
		assert(ss1.str() == ans);

		tree.remove(2);

		auto ans2 =
			R"(
4 1 0 -1 3 5 6 
-1 0 1 3 4 5 6 
4 0 -1 1 5 6 
-1 0 1 4 5 6 
4 0 -1 1 6 
-1 0 1 4 6 
)";
		ss3 << std::endl;
		tree.traversal_recursive<Order::PreOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;
		tree.traversal_recursive<Order::InOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;

		tree.remove(3);

		tree.traversal_recursive<Order::PreOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;
		tree.traversal_recursive<Order::InOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;

		tree.remove(5);

		tree.traversal_recursive<Order::PreOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;
		tree.traversal_recursive<Order::InOrder>([&ss3](auto& a)
		{
			ss3 << a << " ";
		});
		ss3 << std::endl;

		assert(ss3.str() == ans2);



		auto tree2 = avl_tree<std::unique_ptr<int>, ptr_int_cmp<std::unique_ptr<int>>>();
		tree2.insert(std::make_unique<int>(1));
		tree2.insert(std::make_unique<int>(3));
		tree2.insert(std::make_unique<int>(2));
		tree2.insert(std::make_unique<int>(6));
		tree2.insert(std::make_unique<int>(5));

		auto& uniptr_3 = tree2.search(3);
		assert(3 == *uniptr_3);

		auto tree3 = avl_tree<int, std::less<>, nop_unique_ptr, nop_ptr_maker>();
		tree3.insert(3);
		tree3.insert(4);
		tree3.insert(5);
		tree3.insert(6);
		tree3.insert(2);
		tree3.insert(1);
		tree3.insert(0);
		tree3.insert(-1);

		ss2 << std::endl;
		tree3.traversal_recursive<Order::PreOrder>([&ss2](auto& a)
		{
			ss2 << a << " ";
		});
		ss2 << std::endl;
		tree3.traversal_recursive<Order::InOrder>([&ss2](auto& a)
		{
			ss2 << a << " ";
		});
		ss2 << std::endl;
		assert(ss2.str() == ans);

		tree3.remove(4);

		auto ans3 =
			R"(
2 0 -1 1 4 3 6 
-1 0 1 2 3 4 6 
2 0 -1 1 4 3 6 5 
-1 0 1 2 3 4 5 6 
2 0 -1 1 5 3 6 
-1 0 1 2 3 5 6 
)";

		auto tree4 = avl_tree<int>();
		tree4.insert(3);
		tree4.insert(4);
		tree4.insert(5);
		tree4.insert(6);
		tree4.insert(2);
		tree4.insert(1);
		tree4.insert(0);
		tree4.insert(-1);

		tree4.remove(5);

		auto t4_it = tree4.begin();
		assert(*t4_it == -1);
		assert(*++t4_it == 0);
		assert(*t4_it++ == 0);
		assert(*t4_it == 1);

		assert(*(t4_it + 2) == 3);

		std::vector<int> to_vector;
		copy(tree4.begin(), tree4.end(),
			back_inserter(to_vector));

		assert(to_vector.size() == 7);

		assert(std::is_partitioned(tree4.begin(), tree4.end(), [](auto i) { return i < 4; }));

		auto sum = 0;
		for(auto x: tree4) {
			sum += x;
		}
		assert(-1 + 0 + 1 + 2 + 3 + 4 + 6 == sum);
		ss4 << std::endl;
		tree4.traversal_recursive<Order::PreOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;
		tree4.traversal_recursive<Order::InOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;

		tree4.insert(5);

		tree4.traversal_recursive<Order::PreOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;
		tree4.traversal_recursive<Order::InOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;

		tree4.remove(4);

		tree4.traversal_recursive<Order::PreOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;
		tree4.traversal_recursive<Order::InOrder>([&ss4](auto& a)
		{
			ss4 << a << " ";
		});
		ss4 << std::endl;
		assert(ans3 == ss4.str());

		std::vector<int> v(100);
		iota(begin(v), end(v), 0);
		auto tree5 = avl_tree<int>();

		std::random_device rd;
		std::mt19937 g(rd());
		shuffle(v.begin(), v.end(), g);
		for (auto in : v) {
			tree5.insert(in);
		}
		assert(tree5.rank(9) == 9);
		assert(tree5.nth(9) == 9);
		assert(tree5.nth(12) == 12);
		assert(tree5.remove(9));
		assert(tree5.nth(9) == 10);
		assert(tree5.nth(23) == 24);
		assert(!tree5.remove(9));
		assert(tree5.nth(9) == 10);
		assert(tree5.insert(9));
		assert(!tree5.insert(9));
		assert(!tree5.insert(20));

		try {
			tree5.search(1000);
			throw std::runtime_error("std::out_of_range expected");
		}
		catch (std::out_of_range& e) {
			assert(std::string(e.what()) == "not found");
		}

		try {
			tree5.nth(1000);
			throw std::runtime_error("std::out_of_range expected");
		}
		catch (std::out_of_range& e) {
			assert(std::string(e.what()) == "too large");
		}

		std::uniform_int_distribution<int> dis(0, 99);
		while (tree5.size() > 50) {
			tree5.remove(dis(g));
		}

		using psi = std::pair<size_t, int>;
		struct psi_cmp
		{
			bool operator()(psi const& a, size_t b) const
			{
				return a.first < b;
			}

			bool operator()(size_t a, psi const& b) const
			{
				return a < b.first;
			}

			bool operator()(psi const& a, psi const& b) const
			{
				return a.first < b.first;
			}
		};
		auto psi_tree = avl_tree<psi, psi_cmp>();
		psi_tree.insert(std::make_pair(1U, -1));
		psi_tree.insert(std::make_pair(2U, 2));
		psi_tree.insert(std::make_pair(3U, -3));
		psi_tree.insert(std::make_pair(4U, 4));
		psi_tree.search_pair_key(3) = 3;
		assert(psi_tree.search_pair_key(1) == -1);
		assert(psi_tree.search(3).second == 3);

		auto s1 = std::make_shared<int>(1);
		auto s2 = std::make_shared<int>(2);
		auto s3 = std::make_shared<int>(3);
		auto s4 = std::make_shared<int>(4);
		auto s5 = std::make_shared<int>(5);
		auto s6 = std::make_shared<int>(6);
		{
			//, ptr_int_cmp<std::shared_ptr<int>
			auto shared_tree = avl_tree<std::shared_ptr<int>, ptr_int_cmp<std::shared_ptr<int>>>();


			shared_tree.insert(std::shared_ptr<int>(s1));
			shared_tree.insert(std::shared_ptr<int>(s2));
			shared_tree.insert(std::shared_ptr<int>(s3));
			shared_tree.insert(std::shared_ptr<int>(s4));
			shared_tree.insert(std::shared_ptr<int>(s5));
			shared_tree.insert(std::shared_ptr<int>(s6));

			assert(shared_tree.search(3) == s3);

			assert(s1.use_count() == 2);
		}
		assert(s1.use_count() == 1);
	}
#ifdef Use_Wcout
	std::wcout << L"AVL 测试完成" << std::endl;
#else //Use_Wcout
	std::cout << "AVL test complete" << std::endl;
#endif //Use_Wcout
	//++End AVL test
#endif

	return 0;
}
