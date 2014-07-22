//
//  Ex_003_EnvironmentMapping.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 22/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_003_EnvironmentMapping__
#define __Three_cpp_Rev_2__Ex_003_EnvironmentMapping__

#include <iostream>
#include "Example.h"

namespace three {
    class Ex_003_EnvironmentMapping: public Example {
    public:
        static ptr<Ex_003_EnvironmentMapping> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_003_EnvironmentMapping__) */
