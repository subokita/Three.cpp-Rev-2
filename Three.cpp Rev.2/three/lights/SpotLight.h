//
//  SpotLight.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 11/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__SpotLight__
#define __Three_cpp_Rev_2__SpotLight__

#include <iostream>
#include "Light.h"

namespace three {
    class SpotLight : public Light {
    friend class Renderer;

    public:
        static ptr<SpotLight> create( const Color color, const float intensity, const float distance, const float angle, const float exponent );
        ~SpotLight();
        
        /** Constructors **/
        SpotLight();
        SpotLight( const Color color, const float intensity, const float distance, const float angle, const float exponent );

        
        float distance;
        float angle;
        float exponent;
    };
}

#endif /* defined(__Three_cpp_Rev_2__SpotLight__) */
