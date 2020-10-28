# Prophet

Prophet is feedforward convolution neural network all inclusive written in C++.
It has no dependencies whatsoever.

It is made taking into account performance but also compatiblity across different hardware.
For this reason is not using CUDA, which  would be faster but constrained, but is using a full implementation in SIMD SSE2/AVX/AVX512.

Also is trying to push metatemplate programming and use of compile time branch and inheritance, to improve the performance.

Because I also wanted to use this in a sort of game oriented real time environment, even if is far away from this target at the moment, this library support only integer and float types.

In the future this library should be a full CNN library.


## Specification

Because Prophet has made thinking to a "game oriented" environment, the only 2 types allowed are float and int.
This helped me during the writing of the SIMD helper to support the different packet size.


## Types

Is possible to define different type, even if they are all dynamic in general way, only the Tensor is a dynamic matrix


```cpp
// DECLARATION AND DEFINITION
Scalar<int> scalar_int = 4;
Scalar<float> scalar_int = 2.5f;

Vector<int, 3> vector3_int { {1, 2, 3} };
Vector<int, 10> vector3_int { {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };
Vector<float, 3> vector3_float { {1.0f, 2.0f, 3.0f} };
Vector<float, 10> vector3_float { {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f} };

Matrix<int, 2, 3> matrix2x3_int { { { 1, 2, 3}, { 4, 5, 6} } };
Matrix<float, 2, 3> matrix2x3_float { { { 1.0f, 2.0f, 3.0f}, { 4.0f, 5.0f, 6.0f} } };

Tensor<int, 1, 2, 3> tensor1x2x3_int { { { { 10, 20, 30}, { 40, 50, 60} } } };
Tensor<int, 1, 2, 3, 4> tensor1x2x3x4_int{ { { { { 10, 20, 30, 40}, { 50, 60, 70, 80}, { 90, 100, 110, 120} }, { { 11, 22, 33, 44}, { 55, 66, 77, 88}, { 99, 110, 121, 132} } } } };
Tensor<float, 1, 2, 3> tensor1x2x3_float { { { { 10.0f, 20.0f, 30.0f}, { 40.0f, 50.0f, 60.0f} } } };
Tensor<float, 1, 2, 3, 4> tensor1x2x3x4_float{ { { { { 10.0f, 20.0f, 30.0f, 40.0f}, { 50.0f, 60.0f, 70.0f, 80.0f}, { 90.0f, 100.0f, 110.0f, 120.0f} }, { { 11.0f, 22.0f, 33.0f, 44.0f}, { 55.0f, 66.0f, 77.0f, 88.0f}, { 99.0f, 110.0f, 121.0f, 132.0f} } } } };
```


## Example

There is a Test project and folder where you can find some test to run.


# Build Status

| Platform | Build Status |
|:--------:|:------------:|
| Windows (Visual Studio 2019) | [![Windows Build Status](https://ci.appveyor.com/api/projects/status/github/kabalmcblade/prophet?branch=main&svg=true)](https://ci.appveyor.com/project/kabalmcblade/prophet) |