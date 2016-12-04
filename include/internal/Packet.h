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

#pragma once

#define PACKET_BUFFER_SIZE 200
#define PACKET_HEADER_SIZE 4

class Packet
{
public:
    Packet();

    bool getIsFirst() const;
    void setIsFirst(bool f);
    bool getIsLast() const;
    void setIsLast(bool f);
    bool getIsValid() const;

    int getCounter() const;
    void setCounter(int v);

    unsigned char flags;
    unsigned char counter[3];
    unsigned char buffer[PACKET_BUFFER_SIZE];
};
