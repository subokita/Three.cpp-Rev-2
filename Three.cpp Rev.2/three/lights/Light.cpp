//
//  Light.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Light.h"
#include "three.h"

using namespace std;

namespace three {
    Light::Light():
        color    ( 0xFFFFFF ),
        intensity( 1.0 ),
        target( Object3D::create() )
    {}
    
    Light::Light( const Color color, const float intensity ) :
        color    (color ),
        intensity(intensity),
        target( Object3D::create() )
    {}
    
    Light::~Light(){}
}