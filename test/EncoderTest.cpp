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
#include <vector>
#include <stdio.h>
#include <memory.h>

#define CHECK(expression) do { if(expression == 0) return -1; } while(false)

struct EncoderTestData
{
    std::vector<std::vector<unsigned char> > buffer;
};

ErrorCode test_sender(void* userData, const void* buffer, int n)
{
    EncoderTestData* data = (EncoderTestData*)userData;

    std::vector<unsigned char> b(n);
    std::copy((unsigned char*)buffer, (unsigned char*)buffer + n, b.begin());

    data->buffer.push_back(b);
    return SUCCESS;
}

ErrorCode test_error_sender(void* userData, const void* buffer, int n)
{
    return SENDER_ERROR;
}

int EncoderTest( int argc, char *argv[] )
{
    EncoderTestData data;

    Encoder encoder(test_sender, &data);

    const int sourceBufferSize = 1234;

    std::vector<unsigned char> sourceBuffer(sourceBufferSize);
    for (size_t i = 0; i < sourceBuffer.size(); i++)
    {
        sourceBuffer[i] = (unsigned char)i;
    }

    CHECK(encoder.send(0, -1) != SUCCESS);
    CHECK(encoder.send(&sourceBuffer[0], -1) != SUCCESS);
    CHECK(encoder.send(&sourceBuffer[0], sourceBuffer.size()) == SUCCESS);

    CHECK(data.buffer.size() == ( (sourceBufferSize - 1) / PACKET_BUFFER_SIZE + 1));

    int offset = 0;
    for (size_t i = 0; i < data.buffer.size(); i++)
    {
        // Check packet header
        unsigned char flags = data.buffer[i][0];
        bool isFirst = !!(flags & 1);
        bool isLast = !!(flags & 2);
        unsigned char reserved = flags & 0xFC;
        
        if (i == 0)
        {
            CHECK(isFirst);
        }
        else
        {
            CHECK(!isFirst);
        }

        if (i == data.buffer.size()-1)
        {
            CHECK(isLast);
        }
        else
        {
            CHECK(!isLast);
        }
        CHECK(reserved == 0);

        int counter = (data.buffer[i][1] << 16) + (data.buffer[i][2] << 8) + data.buffer[i][3];
        CHECK(counter == i);
        
        // Manage case when the length of the is not a multiple of the packet size
        size_t l = data.buffer[i].size() - 4;
        if (sourceBuffer.size() - offset < l)
        {
            l = sourceBuffer.size() - offset;
        }

        // Compare with input data
        CHECK(memcmp(&data.buffer[i][0] + 4, &sourceBuffer[offset], l) == 0);
        offset += data.buffer[i].size() - 4;
    }

    Encoder encoderError(test_error_sender, &data);
    CHECK(encoderError.send(&sourceBuffer[0], 5) == SENDER_ERROR);

	return 0;
}
