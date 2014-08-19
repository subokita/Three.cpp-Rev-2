//
//  HemisphereLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 11/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "HemisphereLight.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<HemisphereLight> HemisphereLight::create(const Color sky_color, const Color ground_color, const float intensity){
        return make_shared<HemisphereLight>(HemisphereLight(sky_color, ground_color, intensity));
    }
    
    HemisphereLight::HemisphereLight() {
        this->position = glm::vec3(0.0, 100.0, 0.0 );
    }
    
    HemisphereLight::HemisphereLight(const Color sky_color, const Color ground_color, const float intensity):
        Light       ( sky_color, intensity ),
        groundColor ( ground_color )
    {
        this->position  = glm::vec3(0.0, 100.0, 0.0 );
    }
    
    HemisphereLight::~HemisphereLight(){}
    
    const Color& HemisphereLight::getGroundColor() {
        return groundColor;
    }
}