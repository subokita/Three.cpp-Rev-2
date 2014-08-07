//
//  Ex_006_BoundingBoxTest.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 06/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_006_BoundingBoxTest__
#define __Three_cpp_Rev_2__Ex_006_BoundingBoxTest__

#include <iostream>
#include "Example.h"

namespace three {
    class Ex_006_BoundingBoxTest: public Example {
    public:
        static ptr<Ex_006_BoundingBoxTest> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_006_BoundingBoxTest__) */
