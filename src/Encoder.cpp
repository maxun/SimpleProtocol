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

#include "Encoder.h"
#include <algorithm>


Encoder::Encoder(sender_cb sender, void* user)
{
    _sender = sender;
    _user = user;
    _counter = 0;
}

ErrorCode Encoder::send(const unsigned char* buffer, int n)
{
    if (buffer == 0 || n <= 0)
    {
        return INVALID_PARAM;
    }

    int offset = 0;
    _packet.setIsFirst(true);
    for (int offset = 0; offset < n; offset += PACKET_BUFFER_SIZE)
    {
        _packet.setCounter(_counter);
        _counter = (_counter + 1) & 0x00FFFFFF;

        int packetSize = PACKET_BUFFER_SIZE;
        if (n - offset < packetSize)
        {
            _packet.setIsLast(true);
            // Set the remaining byte to 0
            packetSize = n - offset;
            std::fill(&_packet.buffer[packetSize], &_packet.buffer[packetSize] + PACKET_BUFFER_SIZE - packetSize, 0);
        }
        std::copy(&buffer[offset], &buffer[offset] + packetSize, &_packet.buffer[0]);

        ErrorCode errorCode = _sender(_user, &_packet, PACKET_BUFFER_SIZE + PACKET_HEADER_SIZE);
        if (errorCode != SUCCESS)
        {
            return errorCode;
        }
        _packet.setIsFirst(false);
    }

    return SUCCESS;
}

// Public C function

Encoder* encoder_create(sender_cb sender, void* userData)
{
    return new Encoder(sender, userData);
}

ErrorCode encoder_send(Encoder* encoder, const unsigned char* buffer, int length)
{
    return encoder->send(buffer, length);
}

void encoder_release(Encoder** decoder)
{
    delete *decoder;
    *decoder = 0;
}
