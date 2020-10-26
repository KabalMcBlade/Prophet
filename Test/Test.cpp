// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../Prophet/Prophet.h"

USING_PROPHET

int main()
{
	// NOTE testF is the one used everywhere!

	//Matrix<std::string> a;	// static assert works
	Matrix<float, 2, 0> testA;
	Matrix<float, 2, 3> testB;
	Matrix<float, 2, 5> testC;
	Matrix<float, 6, 3> testD;
	Matrix<float, 3, 7> testE;
	Matrix<float, 5, 10> testF;
	Vector<float, 7> testVecA;
	testVecA.SetRandomUniformDistribution();
	//testVecA.Print();

	testB.SetRandomUniformDistribution();
	testC.SetRandomUniformDistribution();
	testD.SetRandomUniformDistribution();
	testF.SetRandomUniformDistribution();

	testF.Print();

	Utils::SimdHelper<float>::Type values0 = testF.GetRow<0, 0>();
	Utils::SimdHelper<float>::Type values1 = testF.GetRow<0, 1>();
	Utils::SimdHelper<float>::Type values2 = testF.GetRow<0, 2>();

	const float* valuesRowAddr = testF.GetRowAddress<0>(1);
	const float* valuesRowAddrT = testF.GetRowAddress<0, 1>();
	const float* fullValuesRowAddr = testF.GetRowAddress(0);
	const float* fullValuesAddr = testF.GetAddress();

	float _0_0 = Utils::SimdHelper<float>::GetValueByIndex<0>(values0);
	float _0_4 = Utils::SimdHelper<float>::GetValueByIndex<3>(values0);

	Matrix<float, 5, 10> resultF = LinearAlgebra::Multiply<float, 5, 10>(testF, 10.0f);

	resultF.Print();
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
