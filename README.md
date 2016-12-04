# SimpleProtocol

[![license](https://img.shields.io/github/license/mashape/apistatus.svg)]()

SimpleProtocol is the implementation of a very simple protocol to transport a stream of compressed images over a network.
The library is written in C++ but only C functions are exposed making it usable by C and C++ programs.

## Requirements
> - C++ compiler (tested with g++ 4.8.4 and Visual Studio 2010)
> - CMake 2.8 (https://cmake.org/).
> - Optional: Boost (http://www.boost.org) is required for the udp client/server sample

## Installation and compilation
SimpleProtocol has been tested on Linux and Windows. The following instructions are for Linux and must be adapted for Windows.

> git clone https://github.com/maxun/SimpleProtocol
> cd SimpleProtocol
> mkdir build
> cmake ..
> make

## Testing

To run unit-tests, first you must compile SimpleProtocol as described above. Then run:

> make test

Should display:

>  Test project SimpleProtocol/build
>      Start 1: PacketTest
>  1/4 Test #1: PacketTest .......................   Passed    0.02 sec
>      Start 2: DecoderTest
>  2/4 Test #2: DecoderTest ......................   Passed    0.00 sec
>      Start 3: EncoderTest
>  3/4 Test #3: EncoderTest ......................   Passed    0.00 sec
>      Start 4: PacketPoolTest
>  4/4 Test #4: PacketPoolTest ...................   Passed    0.00 sec
>
>  100% tests passed, 0 tests failed out of 4
>
>  Total Test time (real) =   0.05 sec>Total Test time (real) =   0.10 sec

## Samples

Two samples are available: a basic example in C and a more advanced example of a udp client/server

