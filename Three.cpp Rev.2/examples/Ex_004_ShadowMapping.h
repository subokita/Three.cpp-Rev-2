//
//  Ex_004_ShadowMapping.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 03/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_004_ShadowMapping__
#define __Three_cpp_Rev_2__Ex_004_ShadowMapping__

#include <iostream>
#include "Example.h"

namespace three {
    class Ex_004_ShadowMapping: public Example {
    public:
        static ptr<Ex_004_ShadowMapping> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_004_ShadowMapping__) */
