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
#include <algorithm>

Packet::Packet()
{
    flags = 0;
    std::fill(counter, counter + sizeof(counter), 0);
    std::fill(buffer, buffer +sizeof(buffer), 0);
}

bool Packet::getIsFirst() const
{ 
    return (flags & 1) != 0; 
}

void Packet::setIsFirst(bool f)
{
    flags = f ? flags | 1 : flags & 0xfe;
}

bool Packet::getIsLast() const
{
    return (flags & 2) != 0;
}

void Packet::setIsLast(bool f)
{
    flags = f ? flags | 2 : flags & 0xfd;
}

bool Packet::getIsValid() const
{
    return !(flags & 0xFC);
}

int Packet::getCounter() const
{ 
    return counter[2] + (counter[1] << 8) + (counter[0] << 16); 
}

void Packet::setCounter(int v)
{
    counter[0] = (v >> 16) & 0xff;;
    counter[1] = (v >> 8) & 0xff;
    counter[2] = v & 0xff;
}
