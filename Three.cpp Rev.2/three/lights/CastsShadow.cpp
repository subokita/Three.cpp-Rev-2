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
        shadowBias(-0.0001),
        shadowDarkness(0.55)
    {
    }
    
    CastsShadow::~CastsShadow() {
        
    }
}