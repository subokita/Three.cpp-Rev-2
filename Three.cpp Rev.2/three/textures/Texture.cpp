//
//  Texture.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Texture.h"


namespace three {
    
    Texture::Texture() :
        HasID    ( textureIDCount++ ),
        textureID( 0 ),
        width    ( 0 ),
        height   ( 0 )
    {}
    
    Texture::~Texture(){

    }
}