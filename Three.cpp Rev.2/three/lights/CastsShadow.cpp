//
//  CastsShadow.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 24/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CastsShadow.h"

namespace three {
    
    CastsShadow::CastsShadow() :
        shadowBias        (-0.0001),
        shadowDarkness    (0.55),
        shadowMapSize     (512.0, 512.0),
        shadowCameraFOV   (60.0),
        shadowCameraLeft  (-10.0),
        shadowCameraRight ( 10.0),
        shadowCameraTop   ( 10.0),
        shadowCameraBottom(-10.0),
        shadowCameraNear  ( -5.0),
        shadowCameraFar   ( 10.0)
    {
    }
    
    CastsShadow::~CastsShadow() {
        
    }
}