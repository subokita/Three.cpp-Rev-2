//
//  VirtualLight.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "VirtualLight.h"
#include "MathUtils.h"

using std::vector;

namespace three {
    
    ptr<VirtualLight> VirtualLight::create(){
        return make_shared<VirtualLight>();
    }
    
    VirtualLight::VirtualLight():
        pointsWorld  (vector<glm::vec3>(8, glm::vec3(0.0)) ),
        pointsFrustum(vector<glm::vec3>(8, glm::vec3(0.0)) )
    {
        
    }
    
    VirtualLight::~VirtualLight(){}
    
    
    void VirtualLight::lookAt( const float x, const float y, const float z ) {
        this->lookAt(glm::vec3(x, y, z));
    }
    
    void VirtualLight::lookAt(const glm::vec3 vec ) {
        this->matrix = glm::lookAt( this->position, vec, this->up );
        MathUtils::decomposeMatrix( this->matrix, this->position, this->quaternion, this->scale );
    }
}