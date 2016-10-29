// ReSharper disable CppUnusedIncludeDirective
#include "Expression.hpp"
#include "SparseMatrix.hpp"
#include "BFS.hpp"

#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>

int main()
{
	try{
#ifdef _WIN32
		std::wcout.imbue(std::locale("chs"));
#else
		std::wcout.imbue(std::locale("zh_CN.UTF-8"));
#endif
	}catch(std::runtime_error){
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

#ifdef BFS_enabled
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
		auto w = BFS_pretty_graph(x, 5, 5, map);
		std::stringstream ss,ss2;
		ss << w;
		auto ans =
R"(FW   
DW W 
RRRRD
 WWWD
   WF
)";
		ss2 << ans;
		assert(ss.str() == ss2.str());
	}
	std::wcout << L"BFS 测试完成" << std::endl;
	//++End BFS test
#endif

#ifdef Expression_enabled
	//++Start Expression test
	{
		auto str = "1+2+   (40*34-22)*2";
		auto exp = GetExp(str);
		assert(exp->Eval() == 2679.0);

		auto str1 = "2-3*4-5/6";
		auto exp1 = GetExp(str1);
		assert(fabs(exp1->Eval() - (2-3*4-5.0/6)) < 0.01);
		
		auto str2 = "2-3*4-5/6 + 7*(8-   (9*10+1)/2 +20+2*10    ) -20 +12";
		auto exp2 = GetExp(str2);
		assert(fabs(exp2->Eval() - (2-3*4-5.0/6+7*(8-(9*10+1.0)/2 +20+2*10)-20+12)) < 0.01);
	}
	std::wcout << L"Expression 测试完成" << std::endl;
	//++End Expression test
#endif
	return 0;
}
