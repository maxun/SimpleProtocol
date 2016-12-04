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

#ifndef __SimpleProtocol_header__
#define __SimpleProtocol_header__

#define SIMPLEPROTOCOL_VERSION_MAJOR 1
#define SIMPLEPROTOCOL_VERSION_MINOR 0

#pragma warning (disable : 4273 )

#if defined(WIN32)
  #ifdef SIMPLEPROTOCOL_EXPORTS  
    #define SIMPLEPROTOCOL_API __declspec(dllexport)   
  #else  
    #define SIMPLEPROTOCOL_API __declspec(dllimport)   
  #endif
#else
  #define SIMPLEPROTOCOL_API
#endif

typedef enum
{
    SUCCESS,
    INVALID_PARAM,
    PROTOCOL_ERROR,
    SENDER_ERROR,
    RECEIVER_ERROR
} ErrorCode;

#define SIMPLEPROTOCOL_PACKET_SIZE 204

#ifdef __cplusplus
class Encoder;
class Decoder;

extern "C" {
#else
typedef struct Encoder_t Encoder;
typedef struct Decoder_t Decoder;
#endif


typedef ErrorCode(*sender_cb)(void* userData, const void* buffer, int n);
typedef ErrorCode(*receiver_cb)(void* userData, void* buffer, int n);


SIMPLEPROTOCOL_API Encoder* encoder_create(sender_cb sender, void* userData);
SIMPLEPROTOCOL_API ErrorCode encoder_send(Encoder* encoder, const unsigned char* buffer, int length);
SIMPLEPROTOCOL_API void encoder_release(Encoder** encoder);

SIMPLEPROTOCOL_API Decoder* decoder_create(receiver_cb receiver, void* userData);
SIMPLEPROTOCOL_API ErrorCode decoder_receive(Decoder* decoder, unsigned char* buffer, int* length);
SIMPLEPROTOCOL_API void decoder_release(Decoder** decoder);

#ifdef __cplusplus
}
#endif
#endif
