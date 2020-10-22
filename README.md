# Prophet

Prophet is feedforward convolution neural network all inclusive written in C++.
It has no dependencies whatsoever.

It is made taking into account performance but also compatiblity across different hardware.
For this reason is not using CUDA, which  would be faster but constrained, but is using a full implementation in SIMD SSE2/AVX/AVX512.

Also is trying to push metatemplate programming and use of compile time branch and inheritance, to improve the performance.

Because I also wanted to use this in a sort of game oriented real time environment, even if is far away from this target at the moment, this library support only integer and float types.

In the future this library should be a full CNN library.
