#include <iostream>
#include <cassert>
#include <sstream>
#include "SparseMatrix.hpp"
int main()
{
	//++Start SparseMatrix2 test
	{
		auto mat = SparseMatrix2<int, 2, 3>();
		mat.set<0, 0>(1);
		mat.set<0, 1>(1);
		mat.set<1, 2>(4);
		auto mat2 = SparseMatrix2<int, 3, 1>();
		mat2.set<0, 0>(4);
		mat2.set<1, 0>(1);
		mat2.set<2, 0>(2);
		auto mat3 = mat.Mul(mat2);
		auto mat4 = mat3.Rev();

		assert((mat3.get<0, 0>() == 14));
		assert((mat3.get<1, 0>() == 28));
		assert((mat4.get<0, 0>() == 14));
		assert((mat4.get<0, 1>() == 28));
		//assert((mat4.get<0, 2>() == 28)); Shall Not Compile

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
	std::cout << "SparseMatrix2 Test Completed" << std::endl;
	//++ End SparseMatrix2 test
	

	return 0;
}