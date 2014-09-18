//
//  Ex_001_SimplePrimitives.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_001_SimplePrimitives__
#define __Three_cpp_Rev_2__Ex_001_SimplePrimitives__

#include <iostream>
#include "Example.h"

namespace three {
    /**
     * Simple primitive examples
     */
    class Ex_001_SimplePrimitives: public Example {
    public:
        static ptr<Ex_001_SimplePrimitives> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_001_SimplePrimitives__) */
