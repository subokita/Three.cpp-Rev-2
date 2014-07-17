//
//  AmbientLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__AmbientLight__
#define __Three_cpp_Rev_2__AmbientLight__

#include <iostream>
#include "Light.h"

namespace three {
    class AmbientLight: public Light {
    public:
        static PTR(AmbientLight) create();
        static PTR(AmbientLight) create(Color color);

        
        /** Constructors **/
        AmbientLight();
        AmbientLight(Color& color);
        ~AmbientLight();        
    };
}

#endif /* defined(__Three_cpp_Rev_2__AmbientLight__) */
