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

#define CHECK(expression) do { if(expression == 0) return -1; } while(false)

int PacketPoolTest( int argc, char *argv[] )
{
    Packet* p;
    PacketPool pool(8);


    CHECK(pool.find(45) == 0);

    CHECK(pool.grab(&p) == SUCCESS);

    CHECK(pool.setCounter(p, 45) == SUCCESS);

    CHECK(pool.find(45) != 0);

    CHECK(pool.release(p) == SUCCESS);

    CHECK(pool.find(45) == 0);

    // Grab all available
    for (int i = 0; i < 8; i++)
    {
        CHECK(pool.grab(&p) == SUCCESS);
        CHECK(pool.setCounter(p, i) == SUCCESS);
    }

    // No more available
    CHECK(pool.grab(&p) != SUCCESS);

    // release all
    for (int i = 0; i < 8; i++)
    {
        CHECK((p = pool.find(i)) != 0);
        CHECK(pool.release(p) == SUCCESS);
    }   

    // Invalid
    CHECK(pool.setCounter((Packet*)&pool, 45) != SUCCESS);
    CHECK(pool.release((Packet*)&pool) != SUCCESS);

	return 0;
}
