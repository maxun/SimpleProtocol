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

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "SimpleProtocol.h"

using boost::asio::ip::udp;

ErrorCode udp_sender(void* userData, const void* buffer, int n)
{
    try
    {
        boost::asio::io_service io_service;

        udp::socket socket(io_service);
        socket.open(udp::v4());

        socket.send_to(boost::asio::buffer(buffer, n), udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888));
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return SENDER_ERROR;
    }
    return SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: udp_client <filename>" << std::endl;
        return 1;
    }

    // Read file
    std::ifstream fin(argv[1], std::ifstream::binary);
    fin.seekg(0, std::ifstream::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ifstream::beg);

    if (size == -1)
    {
        std::cerr << "Error while reading file " << argv[1] << std::endl;
        return 1;
    }

    if (size > 5 * 1024 * 1024)
    {
        std::cerr << "File too big" << std::endl;
        return 1;
    }
    std::vector<unsigned char> buffer(size);
    fin.read((char*)&buffer[0], size);
    fin.close();

    // User SimpleProtocol to send the file
    Encoder* encoder = encoder_create(udp_sender, 0);
    ErrorCode errorCode = encoder_send(encoder, &buffer[0], size);
    if(errorCode != SUCCESS)
    {
        std::cerr << "encoder_send failed" << std::endl;        
    }
    encoder_release(&encoder);


    return 0;
}