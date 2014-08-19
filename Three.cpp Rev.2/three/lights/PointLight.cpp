//
//  PointLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PointLight.h"
#include "ShaderLib.h"
#include "Shader.h"


namespace three {
    ptr<PointLight> PointLight::create( const glm::vec3 position, const Color color, const float intensity, const float distance ){
        return make_shared<PointLight>(PointLight(position, color, intensity, distance));
    }
    
    /** Constructors **/
    PointLight::PointLight():
        distance ( 0.0 )
    {}
    
    PointLight::PointLight( const glm::vec3 position, const Color color, const float intensity, const float distance ):
        Light    ( color, intensity ),
        distance ( distance )
    {
        this->position = position;
    }
    
    PointLight::~PointLight(){}
    
    const float PointLight::getDistance() {
        return distance;
    }
}