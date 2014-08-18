//
//  Ex_008_FontStashIntegration.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 17/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_008_FontStashIntegration__
#define __Three_cpp_Rev_2__Ex_008_FontStashIntegration__

#include <iostream>

#include "Example.h"

namespace three {
    class Ex_008_FontStashIntegration: public Example {
    public:
        static ptr<Ex_008_FontStashIntegration> create();
        
        virtual void run() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ex_008_FontStashIntegration__) */
