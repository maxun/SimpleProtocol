/*
    The MIT License
	
    Copyright (c) 2016 Laurent Desmecht.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Packet.h"

#define CHECK(expression) do { if(expression == 0) return -1; } while(false)

int PacketTest( int argc, char *argv[] )
{
    Packet packet;

    packet.setIsFirst(true);
    CHECK(packet.getIsFirst());
    packet.setIsFirst(false);
    CHECK(!packet.getIsFirst());

    packet.setIsLast(true);
    CHECK(packet.getIsLast());
    packet.setIsLast(false);
    CHECK(!packet.getIsLast());

    CHECK(packet.getIsValid());

    packet.setCounter(1234567);
    CHECK(packet.getCounter() == 1234567);

	return 0;
}
