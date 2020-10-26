// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../Prophet/Prophet.h"

USING_PROPHET

int main()
{
	//////////////////////////////////////////////////////////////////////////
	// Test initialization matrices
	//Matrix<std::string> a;	// static assert works
	Matrix<float, 2, 0> testA;
	Matrix<float, 2, 5> testB;
	Matrix<float, 2, 3> testC;
	Matrix<float, 6, 3> testD;
	Matrix<float, 3, 7> testE;

	Matrix<float, 5, 10> testF;
	Matrix<float, 5, 10> testFxF;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test Hadamard product of matrices
	std::cout << "1) Hadamard product of matrices" << std::endl << std::endl;
	testB.SetRandomUniformDistribution();
	testC.SetRandomUniformDistribution();

	std::cout << "testB [" << testB.GetRowsCount() << "][" << testB.GetColumnsCount() << "]" << std::endl;
	testB.Print();

	std::cout << "testC [" << testC.GetRowsCount() << "][" << testC.GetColumnsCount() << "]" << std::endl;
	testC.Print();

	Matrix resultBxC = LinearAlgebra::Multiply(testB, testC);

	std::cout << "testB X testC [" << resultBxC.GetRowsCount() << "][" << resultBxC.GetColumnsCount() << "]" << std::endl;
	resultBxC.Print();

	std::cout << "---------------------------------------------" << std::endl << std::endl;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Test scalar product
	std::cout << "2) Scalar product" << std::endl << std::endl;
	testD.SetRandomUniformDistribution();

	std::cout << "testD [" << testD.GetRowsCount() << "][" << testD.GetColumnsCount() << "]" << std::endl;
	testD.Print();

	constexpr float scalarValueMul = 10.0f;
	std::cout << "Scalar value " << scalarValueMul << std::endl << std::endl;

	Matrix scalarMultiplication = LinearAlgebra::Multiply(testD, scalarValueMul);

	std::cout << "testD x " << scalarValueMul << " [" << scalarMultiplication.GetRowsCount() << "][" << scalarMultiplication.GetColumnsCount() << "]" << std::endl;
	scalarMultiplication.Print();

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
