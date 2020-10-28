// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../Prophet/Prophet.h"

USING_PROPHET

int main()
{
	/////////////////////////////////////////////////////////////////////////

// 	Scalar<int> scalar1 = 4;
// 	Scalar<int> scalar2 = 2;
// 	Scalar<int> scalarSum = scalar1 + scalar2;
// 
// 	std::cout << scalar1 << " + " << scalar2 << " = " << scalarSum << std::endl << std::endl;
// 
// 	Vector<int, 3> vec3 { {1, 2, 3} };
// 	std::cout << vec3[1] << std::endl << std::endl;
// 	std::cout << vec3 << std::endl << std::endl;
// 
// 	Vector<int, 5> vec5_t { {10, 8, 6, 4, 2} };
// 
// 	Vector<int, 5> vec5;
// 	vec5.CopyFrom(vec5_t);
// 	vec5.SetRandomUniformDistribution();
// 	std::cout << vec5[1] << std::endl << std::endl;
// 
// 
// 	Matrix<int, 2, 3> matrix2x3{ { { 1, 2, 3}, { 4, 5, 6} } };
// 	std::cout << matrix2x3[1][2] << std::endl << std::endl;
// 
// 	std::cout << matrix2x3 << std::endl << std::endl;
// 
// 	Tensor<int, 1, 2, 3> tensor1x2x3 { { { { 10, 20, 30}, { 40, 50, 60} } } };
// 	Tensor<int, 1, 2, 3, 4> tensor1x2x3x4{ { { { { 10, 20, 30, 40}, { 50, 60, 70, 80}, { 90, 100, 110, 120} }, { { 11, 22, 33, 44}, { 55, 66, 77, 88}, { 99, 110, 121, 132} } } } };
// 	Tensor<int, 1, 2, 3, 4> tensor1x2x3x4_for_cpy{ { { { { 9, 8, 7, 6}, { 5, 4, 3, 2}, { 17, 13, 19, 23} }, { { 1, 2, 3, 4}, { 4, 3, 2, 1}, { 7, 7, 9, 9} } } } };
// 	tensor1x2x3x4.Clear();
// 	tensor1x2x3x4.CopyFrom(tensor1x2x3x4_for_cpy);
// 
// 	std::cout << std::endl;
// 
// 	std::cout << "Rank: " << tensor1x2x3.Rank() << std::endl;
// 	std::cout << "Rank: " << tensor1x2x3x4.Rank() << std::endl << std::endl;
// 
// 	std::cout << tensor1x2x3 << std::endl << std::endl;
// 	std::cout << tensor1x2x3x4 << std::endl << std::endl;
// 
// 	const Tensor<int, 1, 2, 3, 4>::Param* baseAddr = tensor1x2x3x4;
// 	const int* baseAddr = tensor1x2x3x4;
// 	std::cout << baseAddr;


	constexpr float scalarValueMul = 10.0f;

	//////////////////////////////////////////////////////////////////////////
	// Test Vector scalar product
	Vector<float, 6> scalarVectorProduct;

	std::cout << "1) Vector scalar product" << std::endl << std::endl;
	scalarVectorProduct.SetRandomUniformDistribution();

	std::cout << "Rank: " << scalarVectorProduct.Rank() << std::endl << std::endl;
	std::cout << "Original Vector value" << std::endl << std::endl;
	std::cout << scalarVectorProduct << std::endl;

	std::cout << "Scalar value " << scalarValueMul << std::endl << std::endl;

	Vector<float, 6> scalarVectorMultiplication = LinearAlgebra::Multiply(scalarVectorProduct, scalarValueMul);
	std::cout << "Computed scalar Vector multiplication" << std::endl << std::endl;
	std::cout << scalarVectorMultiplication << std::endl;

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test Vector scalar product
	Vector<float, 6> vectorProductA;
	Vector<float, 6> vectorProductB;

	std::cout << "2) Vectors product" << std::endl << std::endl;
	vectorProductA.SetRandomUniformDistribution();
	vectorProductB.SetRandomUniformDistribution();

	std::cout << "Rank A: " << vectorProductA.Rank() << std::endl << std::endl;
	std::cout << "Rank B: " << vectorProductB.Rank() << std::endl << std::endl;

	std::cout << "Original Vector A value" << std::endl << std::endl;
	std::cout << vectorProductA << std::endl;

	std::cout << "Original Vector B value" << std::endl << std::endl;
	std::cout << vectorProductB << std::endl;

	Vector<float, 6> vectorMultiplication = LinearAlgebra::Multiply(vectorProductA, vectorProductB);
	std::cout << "Computed Vectors multiplication" << std::endl << std::endl;
	std::cout << vectorMultiplication << std::endl;

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test Matrix scalar product
	Matrix<float, 6, 3> scalarMatrixProduct;

	std::cout << "3) Matrix scalar product" << std::endl << std::endl;
	scalarMatrixProduct.SetRandomUniformDistribution();

	std::cout << "Rank: " << scalarMatrixProduct.Rank() << std::endl << std::endl;
	std::cout << "Original Matrix value" << std::endl << std::endl;
	std::cout << scalarMatrixProduct << std::endl;

	std::cout << "Scalar value " << scalarValueMul << std::endl << std::endl;

	Matrix<float, 6, 3> scalarMatrixMultiplication = LinearAlgebra::Multiply(scalarMatrixProduct, scalarValueMul);
	std::cout << "Computed scalar Matrix multiplication" << std::endl << std::endl;
	std::cout << scalarMatrixMultiplication << std::endl;

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test Hadamard product or Matrices product
	Matrix<float, 6, 4> matrixProductA;
	Matrix<float, 6, 4> matrixProductB;

	std::cout << "4) Matrices product" << std::endl << std::endl;
	matrixProductA.SetRandomUniformDistribution();
	matrixProductB.SetRandomUniformDistribution();

	std::cout << "Rank A: " << matrixProductA.Rank() << std::endl << std::endl;
	std::cout << "Rank B: " << matrixProductB.Rank() << std::endl << std::endl;

	std::cout << "Original Matrix A value" << std::endl << std::endl;
	std::cout << matrixProductA << std::endl;

	std::cout << "Original Matrix B value" << std::endl << std::endl;
	std::cout << matrixProductB << std::endl;

	Matrix<float, 6, 4> matricesMultiplication = LinearAlgebra::Multiply(matrixProductA, matrixProductB);
	std::cout << "Computed Hadamard product or Matrices product" << std::endl << std::endl;
	std::cout << matricesMultiplication << std::endl;

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test Hadamard product or Matrices product
	Matrix<float, 6, 8> matrixProductADiff;
	Matrix<float, 6, 4> matrixProductBDiff;

	std::cout << "5) Matrices product of different dimension same rank" << std::endl << std::endl;
	matrixProductADiff.SetRandomUniformDistribution();
	matrixProductBDiff.SetRandomUniformDistribution();

	std::cout << "Rank A: " << matrixProductADiff.Rank() << std::endl << std::endl;
	std::cout << "Rank B: " << matrixProductBDiff.Rank() << std::endl << std::endl;

	std::cout << "Original Matrix A value" << std::endl << std::endl;
	std::cout << matrixProductADiff << std::endl;

	std::cout << "Original Matrix B value" << std::endl << std::endl;
	std::cout << matrixProductBDiff << std::endl;

	Matrix<float, 6, 4> matricesMultiplicationDiff = LinearAlgebra::Multiply(matrixProductADiff, matrixProductBDiff);
	std::cout << "Computed Hadamard product or Matrices product" << std::endl << std::endl;
	std::cout << matricesMultiplicationDiff << std::endl;

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
