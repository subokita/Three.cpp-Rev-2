//
//  Rect.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 31/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Rect.h"

namespace three {
    Rect::Rect() {}
    
    Rect::Rect( float x, float y, float width, float height ):
        x(x),
        y(y),
        width(width),
        height(height)
    {
    }
    Rect::Rect(const Rect& rhs) {
        this->x      = rhs.x;
        this->y      = rhs.y;
        this->width  = rhs.width;
        this->height = rhs.height;
    }
    
    Rect& Rect::operator=(const Rect& rhs) {
        this->x      = rhs.x;
        this->y      = rhs.y;
        this->width  = rhs.width;
        this->height = rhs.height;
        return *this;
    }
    
    Rect::~Rect(){}
}