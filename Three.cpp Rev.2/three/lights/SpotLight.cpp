//
//  SpotLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 11/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "SpotLight.h"

using namespace std;

namespace three {
    
    ptr<SpotLight> SpotLight::create(){
        return make_shared<SpotLight>();
    }
    
    ptr<SpotLight> SpotLight::create( const Color color, const float intensity, const float distance, const float angle, const float exponent ){
        return make_shared<SpotLight>(color, intensity, distance, angle, exponent);
    }
    
    /** Constructors **/
    SpotLight::SpotLight():
        distance(0.0),
        angle(Math::radToDeg(M_PI / 3.0)),
        exponent(10.0),
        target( 0.0, 0.0, 0.0 )
    {
        this->position = glm::vec3( 0.0, 1.0, 0.0 );
    }
    
    SpotLight::SpotLight( const Color color, const float intensity, const float distance, const float angle, const float exponent ):
        Light   ( color, intensity ),
        distance( distance ),
        angle   ( angle ),
        exponent(exponent),
        target( 0.0, 0.0, 0.0 )
    {
        this->position = glm::vec3( 0.0, 1.0, 0.0 );
    }
    
    SpotLight::~SpotLight(){}
}