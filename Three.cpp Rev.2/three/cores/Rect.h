//
//  Rect.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Rect__
#define __Three_cpp_Rev_2__Rect__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class Rect {
    public:
        Rect();
        Rect( float x, float y, float width, float height );
        Rect(const Rect& rhs);
        ~Rect();
        
        Rect& operator=(const Rect& rhs);
        
    public:
        float x;
        float y;
        float width;
        float height;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Rect__) */
