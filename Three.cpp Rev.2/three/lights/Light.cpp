//
//  Light.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Light.h"

namespace three {
    Light::Light():
        color    ( 0xFFFFFF ),
        intensity( 1.0 )
    {}
    
    Light::Light( const Color color, const float intensity ) :
        color    (color ),
        intensity(intensity)
    {}
    
    Light::~Light(){}
    
    
    const Color& Light::getColor() {
        return this->color;
    }
    
    const float Light::getIntensity() {
        return this->intensity;
    }
}