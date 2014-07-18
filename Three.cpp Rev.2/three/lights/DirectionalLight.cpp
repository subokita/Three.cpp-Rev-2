//
//  DirectionalLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "DirectionalLight.h"

using namespace std;
namespace three {
    
    ptr<DirectionalLight> DirectionalLight::create( Color color, float intensity, glm::vec3 position ){
        return make_shared<DirectionalLight>(DirectionalLight(color, intensity, position));
    }
    
    DirectionalLight::DirectionalLight()
    {}
    
    DirectionalLight::DirectionalLight( Color color, float intensity, glm::vec3 position ) :
        Light( color, intensity )
    {
        this->position  = position;
    }
    
    DirectionalLight::~DirectionalLight(){
    }
}