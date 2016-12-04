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

#include "Decoder.h"
#include <algorithm>

Decoder::Decoder(receiver_cb receiver, void* user)
    : _pool(16)
{
    _receiver = receiver;
    _user = user;
    _counter = 0;    
}

ErrorCode Decoder::receive(unsigned char* buffer, int* length)
{
    if (buffer == 0 || length <= 0)
    {
        return INVALID_PARAM;
    }

    int offset = 0;

    while (true)
    {
        Packet* nextPacket;

        nextPacket = _pool.find(_counter);
        if (nextPacket)
        {
            // The next packet are available, we copy it in the destination buffer
            std::copy(&nextPacket->buffer[0], &nextPacket->buffer[0] + PACKET_BUFFER_SIZE, buffer + offset);
            offset += PACKET_BUFFER_SIZE;

            _counter = (_counter + 1) & 0x00FFFFFF;
            
            bool isLast = nextPacket->getIsLast();
            _pool.release(nextPacket);
            if (isLast)
            {
                *length = offset;
                return SUCCESS;
            }
        }
        else
        {
            // Grab the next available packet
            _pool.grab(&nextPacket);
            if (nextPacket)
            {
                ErrorCode errorCode = _receiver(_user, nextPacket, PACKET_BUFFER_SIZE + PACKET_HEADER_SIZE);
                if (errorCode != SUCCESS)
                {
                    return errorCode;
                }
                
                if (!nextPacket->getIsValid())
                {
                    return PROTOCOL_ERROR;
                }
                _pool.setCounter(nextPacket, nextPacket->getCounter());
            }
            else
            {
                // No packet available
                // 
                return PROTOCOL_ERROR;
            }
        }
    }
    return SUCCESS;
}

// Public C function

Decoder* decoder_create(receiver_cb receiver, void* userData)
{
    return new Decoder(receiver, userData);
}

ErrorCode decoder_receive(Decoder* decoder, unsigned char* buffer, int* length)
{
    return decoder->receive(buffer, length);
}

void decoder_release(Decoder** decoder)
{
    delete *decoder;
    *decoder = 0;
}
