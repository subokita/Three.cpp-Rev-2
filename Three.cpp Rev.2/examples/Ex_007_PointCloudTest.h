//
//  Ex_007_PointCloudTest.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 13/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ex_007_PointCloudTest__
#define __Three_cpp_Rev_2__Ex_007_PointCloudTest__

#include <iostream>

#include "Example.h"

namespace three {
    class Ex_007_PointCloudTest: public Example {
    public:
        static ptr<Ex_007_PointCloudTest> create();
        
        virtual void run() override;
    };
}
#endif /* defined(__Three_cpp_Rev_2__Ex_007_PointCloudTest__) */
