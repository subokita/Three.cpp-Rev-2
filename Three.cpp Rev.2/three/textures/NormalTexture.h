//
//  NormalTexture.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 18/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__NormalTexture__
#define __Three_cpp_Rev_2__NormalTexture__

#include <iostream>
#include "internal_headers.h"
#include "Texture.h"

namespace three {
    class NormalTexture : public Texture {
    public:
        NormalTexture();
        ~NormalTexture();
    };
}

#endif /* defined(__Three_cpp_Rev_2__NormalTexture__) */
