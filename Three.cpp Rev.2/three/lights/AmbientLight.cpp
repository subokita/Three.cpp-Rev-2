//
//  AmbientLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "AmbientLight.h"


using namespace std;
namespace three {
    
    ptr<AmbientLight> AmbientLight::create() {
        return make_shared<AmbientLight>();
    }
    
    ptr<AmbientLight> AmbientLight::create(Color color) {
        return make_shared<AmbientLight>( AmbientLight(color) );
    }
    
    /** Constructors **/
    AmbientLight::AmbientLight()
    {}
    
    AmbientLight::AmbientLight(Color& color) :
        Light(color, 1.0)
    {}
    
    AmbientLight::~AmbientLight(){}
}