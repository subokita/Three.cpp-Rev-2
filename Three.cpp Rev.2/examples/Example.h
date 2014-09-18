//
//  Example.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Example__
#define __Three_cpp_Rev_2__Example__

#include <iostream>
#include "internal_headers.h"
#include "three.h"

namespace three {
    /**
     * Abstract Example class, all other examples should inherit this class
     */
    class Example {
    public:
        virtual void run() = 0;
    };
}
#endif /* defined(__Three_cpp_Rev_2__Example__) */
