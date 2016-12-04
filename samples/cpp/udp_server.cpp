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
#include <sstream>
#include <fstream>
#include <boost/asio.hpp>
#include "SimpleProtocol.h"

using boost::asio::ip::udp;

class udp_server_data
{
public:
    udp_server_data()
        : socket(io_service, udp::endpoint(udp::v4(), 8888))
    {
    }

    boost::asio::io_service io_service;
    udp::socket socket;
};

ErrorCode udp_receiver(void* userData, void* buffer, int n)
{
    udp_server_data* data = (udp_server_data*)userData;

    try
    {   
        udp::endpoint remote_endpoint;
        boost::system::error_code error;
        int p = data->socket.receive_from(boost::asio::buffer(buffer, n),
            remote_endpoint, 0, error);

        if (error && error != boost::asio::error::message_size)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return RECEIVER_ERROR;
    }
    return SUCCESS;
}


int main()
{
    std::vector<unsigned char> buffer(5 * 1024 * 1024); // Can received 5M

    udp_server_data data;

    Decoder* decoder = decoder_create(udp_receiver, &data);

    int fileIndex = 0;
    while (true)
    {
        int length = buffer.size();
        ErrorCode errorCode = decoder_receive(decoder, &buffer[0], &length);
        if (errorCode != SUCCESS)
        {
            std::cout << "Failed to received file" << std::endl;
            break;
        }

        // Save the file
        std::stringstream ss;
        ss << fileIndex << ".jp2";

        std::cout << "Save file to " << ss.str() << std::endl;

        std::ofstream fout(ss.str().c_str(), std::ofstream::binary);
        fout.write((char*)&buffer[0], length);
        fout.close();
        fileIndex++;
    }
    decoder_release(&decoder);

    return 0;
}