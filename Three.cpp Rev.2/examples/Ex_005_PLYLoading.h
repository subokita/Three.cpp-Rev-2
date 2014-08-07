//
//  Ex_005_PLYLoading.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 04/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_005_PLYLoading__
#define __Three_cpp_Rev_2__Ex_005_PLYLoading__

#include <iostream>
#include "Example.h"

namespace three {
    class Ex_005_PLYLoading: public Example {
    public:
        static ptr<Ex_005_PLYLoading> create();
        
        virtual void run() override;
    };
}
#endif /* defined(__Three_cpp_Rev_2__Ex_005_PLYLoading__) */
