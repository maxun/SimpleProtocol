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

#include "PacketPool.h"
#include <algorithm>

PacketPool::PacketPool(int capacity)
{
    _packetCounter.resize(capacity, -1);
    _packets.resize(capacity);
}

ErrorCode PacketPool::grab(Packet** packet)
{
    // Find an available packet
    Packet* p = find(-1);
    if (p == 0)
    {
        *packet = 0;
        return PROTOCOL_ERROR;
    }

    *packet = p;
    return SUCCESS;
}

Packet* PacketPool::find(int counter)
{
    for (size_t i = 0; i < _packets.size(); i++)
    {
        if (_packetCounter[i] == counter)
        {
            return &_packets[i];
        }
    }
    // Not found
    return 0;
}

ErrorCode PacketPool::setCounter(Packet* packet, int c)
{
    size_t index = packet - &_packets[0];
    if (index < 0 || index >= _packets.size())
    {
        return INVALID_PARAM;
    }
    _packetCounter[index] = c;
    return SUCCESS;
}

ErrorCode PacketPool::release(Packet* packet)
{
    return setCounter(packet, -1);
}
