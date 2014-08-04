//
//  IFog.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "IFog.h"

namespace three {
    
    IFog::IFog():
        color( Color(0xFFFFFF) )
    {}
    
    IFog::IFog( Color color ) :
        color( color )
    {}
    
    IFog::~IFog(){}
    
    const Color& IFog::getColor() {
        return color;
    }
    
    void IFog::setColor(const Color color) {
        this->color = color;
    }
}