//
//  NormalMap.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 18/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__NormalMap__
#define __Three_cpp_Rev_2__NormalMap__

#include <iostream>
#include "internal_headers.h"
#include "Texture.h"

namespace three {
    class NormalMap : public Texture {
    public:
        NormalMap();
        ~NormalMap();
    };
}

#endif /* defined(__Three_cpp_Rev_2__NormalMap__) */