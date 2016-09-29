#include <iostream>
#include "SparseMatrix.hpp"
int main()
{
	{
		SparseMatrix<double, 10, 20> mat = SparseMatrix<double, 10, 20>();
		mat.insert(2.4,9,10);
		mat.insert<90,10>(2.4);
	}
	std::cout << "Test Completed" << std::endl;
	return 0;
}