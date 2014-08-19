//
//  DirectionalLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 09/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "DirectionalLight.h"
#include "ShaderLib.h"
#include "Shader.h"

namespace three {
    
    ptr<DirectionalLight> DirectionalLight::create() {
        return make_shared<DirectionalLight>();
    }
    
    ptr<DirectionalLight> DirectionalLight::create( Color color, float intensity, glm::vec3 position ){
        return make_shared<DirectionalLight>(DirectionalLight(color, intensity, position));
    }
    
    DirectionalLight::DirectionalLight()
    {
        setTarget(glm::vec3(0.0));
    }
    
    DirectionalLight::DirectionalLight( Color color, float intensity, glm::vec3 position ) :
        Light( color, intensity )
    {
        this->position  = position;
        setTarget(glm::vec3(0.0));
    }
    
    DirectionalLight::~DirectionalLight(){
    }
    
}