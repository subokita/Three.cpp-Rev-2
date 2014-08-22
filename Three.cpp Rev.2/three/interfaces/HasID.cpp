//
//  HasID.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 16/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "HasID.h"
#include "MathUtils.h"

namespace three {
    
    HasID::HasID( const unsigned int id ) :
        id  ( id ),
        uuid( MathUtils::generateUUID() ),
        name( "" )
    {}

    HasID::~HasID() {
        
    }
}