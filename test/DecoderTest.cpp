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
#include <vector>

#define CHECK(expression) do { if(expression == 0) return -1; } while(false)

struct DecoderTestData
{
    unsigned int current;
    std::vector<std::vector<unsigned char> > buffers;    
};

ErrorCode test_receiver(void* userData, void* buffer, int n)
{
    DecoderTestData* data = (DecoderTestData*)userData;
    if (data->current >= data->buffers.size())
    {
        return RECEIVER_ERROR;
    }

    std::copy(data->buffers[data->current].begin(), data->buffers[data->current].end(), (unsigned char*)buffer);
    data->current++;
    return SUCCESS;
}

void fillBuffer(std::vector<unsigned char>& buffer, int counter, bool isFirst, bool isLast)
{
    buffer[0] = 0;
    if (isFirst)
    {
        buffer[0] |= 1;
    }
    if (isLast)
    {
        buffer[0] |= 2;
    }
    buffer[1] = (counter >> 16) & 0xff;
    buffer[2] = (counter >> 8) & 0xff;
    buffer[3] = counter & 0xff;

    int offset = counter * PACKET_BUFFER_SIZE;
    for (int i = 0; i < PACKET_BUFFER_SIZE; i++)
    {
        buffer[i + 4] = (unsigned char)(offset + i);
    }
}

int DecoderTest( int argc, char *argv[] )
{
    DecoderTestData data;

    // Create packets with valid data
    data.current = 0;

    for (int i = 0; i < 10; i++)
    {
        std::vector<unsigned char> buffer(PACKET_BUFFER_SIZE + 4);
        fillBuffer(buffer, i, i == 0, i == 9);
        data.buffers.push_back(buffer);
    }
    // Swap packets to simulate network packet arriving in wrong order
    std::swap(data.buffers[0], data.buffers[2]);
    std::swap(data.buffers[7], data.buffers[9]);

    std::vector<unsigned char> destinationBuffer(16 * PACKET_BUFFER_SIZE);
    Decoder decoder(test_receiver, &data);
    int frameLength = destinationBuffer.size();
    CHECK(decoder.receive(&destinationBuffer[0], &frameLength) == SUCCESS);
    CHECK(frameLength == 10 * PACKET_BUFFER_SIZE);

    for (int i = 0; i < frameLength; i++)
    {
        CHECK(destinationBuffer[i] == (unsigned char)i);
    }

    // Create packets with invalid data
    data.current = 0;
    data.buffers.clear();
    for (int i = 0; i < 10; i++)
    {
        std::vector<unsigned char> buffer(PACKET_BUFFER_SIZE + 4);
        fillBuffer(buffer, i, i == 0, i == 9);
        data.buffers.push_back(buffer);
    }
    // Corrupt by removing one packet
    data.buffers.erase(data.buffers.begin() + 5);
    Decoder decoderCorrupt1(test_receiver, &data);
    CHECK(decoderCorrupt1.receive(&destinationBuffer[0], &frameLength) == RECEIVER_ERROR);

    // Create more packets with invalid data
    data.current = 0;
    data.buffers.clear();
    for (int i = 0; i < 50; i++)
    {
        std::vector<unsigned char> buffer(PACKET_BUFFER_SIZE + 4);
        fillBuffer(buffer, i, i == 0, i % 10 == 9);
        data.buffers.push_back(buffer);
    }
    // Corrupt too but this time we will get an error PROTOCOL_ERROR
    data.buffers.erase(data.buffers.begin() + 5);
    Decoder decoderCorrupt2(test_receiver, &data);
    CHECK(decoderCorrupt2.receive(&destinationBuffer[0], &frameLength) == PROTOCOL_ERROR);

    return 0;
}
