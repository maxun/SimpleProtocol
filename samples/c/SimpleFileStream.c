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

#include "SimpleProtocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

ErrorCode disk_sender(void* user, const void* buffer, int n)
{
    FILE* file = (FILE*)user;
    fwrite(buffer, n, 1, file);
    return SUCCESS;
}

ErrorCode send_to_file(const unsigned char* srcBuffer, int n)
{
    FILE* file = fopen("buffer.tmp", "wb");
    Encoder* encoder = encoder_create(disk_sender, file);
    ErrorCode errorCode = encoder_send(encoder, &srcBuffer[0], n);
    if (errorCode != SUCCESS)
    {
        return errorCode;
    }

    encoder_release(&encoder);
    fclose(file);
    return SUCCESS;
}

ErrorCode disk_receiver(void* user, void* buffer, int n)
{
    FILE* file = (FILE*)user;
    fread(buffer, n, 1, file);
    fclose(file);
    return SUCCESS;
}

ErrorCode receive_from_file(unsigned char* destBuffer, int n)
{
    FILE* file = fopen("buffer.tmp", "rb");
    Decoder* decoder = decoder_create(disk_receiver, file);
    ErrorCode errorCode = decoder_receive(decoder, &destBuffer[0], &n);
    if (errorCode != SUCCESS)
    {
        return errorCode;
    }

    decoder_release(&decoder);
    return SUCCESS;
}
 
int main(int argc, char *argv[])
{
    static const int n = 2048;
    
    unsigned char* srcBuffer = (unsigned char*)malloc(n);
    unsigned char* destBuffer = (unsigned char*)malloc(n + SIMPLEPROTOCOL_PACKET_SIZE);
    int isEqual;
    int i;
    ErrorCode errorCode;

    for (i = 0; i < n; i++)
    {
        srcBuffer[i] = (unsigned char)i;
    }
    errorCode = send_to_file(srcBuffer, n);
    if (errorCode != SUCCESS) return 1;
    
    errorCode = receive_from_file(destBuffer, n);
    if (errorCode != SUCCESS) return 1;

    isEqual = memcmp(destBuffer, srcBuffer, n) == 0;
    if (isEqual)
    {
        printf("Transfert Sucess\n");
    }
    else
    {
        printf("Transfert Failed\n");
    }
    
    free(srcBuffer);
    free(destBuffer);

    return 0;
}
