#include "SparseMatrix.hpp"
#include <cassert>
#include <sstream>
int main()
{
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
	std::cout << "SparseMatrix2 Test Completed" << std::endl;
	//++ End SparseMatrix2 test
	

	return 0;
}