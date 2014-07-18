//
//  Fog.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Fog.h"

using namespace std;

namespace three {
    
    ptr<Fog> Fog::create(){
        return make_shared<Fog>();
    }
    
    ptr<Fog> Fog::create( Color color, float near, float far ){
        return make_shared<Fog>( Fog(color, near, far) );
    }
    
    Fog::Fog()
    {}
    
    Fog::Fog( Color color, float near, float far ):
        IFog( color ),
        near( near ),
        far ( far )
    {}
    
    Fog::~Fog(){}
}