//
//  CastsShadow.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 24/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CastsShadow.h"

using namespace std;

namespace three {
    
    CastsShadow::CastsShadow() :
        shadowBias        (-0.0001),
        shadowDarkness    (0.55),
        shadowMapSize     (512.0, 512.0),
        shadowCameraFOV   (60.0),
        shadowCameraLeft  (-15.0),
        shadowCameraRight ( 15.0),
        shadowCameraTop   ( 15.0),
        shadowCameraBottom(-15.0),
        shadowCameraNear  (-15.0),
        shadowCameraFar   ( 15.0)
    {
    }
    
    CastsShadow::~CastsShadow() {
        
    }
}