//
//  Camera.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Camera.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Math.h"

using namespace std;

namespace three {
    Camera::Camera(){
        up          = glm::vec3(0.0, 1.0, 0.0);
        projection  = glm::mat4(1.0);
    }
    
    Camera::Camera(const glm::vec3 position, const glm::vec3 up) {
        this->position  = position;
        this->up        = up;
        
        projection = glm::mat4(1.0);
        lookAt( glm::vec3(0.0) );
    }
    
    Camera::~Camera(){}
    
    
    void Camera::lookAt( const float x, const float y, const float z ) {
        this->lookAt(glm::vec3(x, y, z));
    }
    
    void Camera::lookAt(const glm::vec3 vec ) {
        this->target = vec;
        this->matrix = glm::lookAt( this->position, vec, this->up );
        Math::decomposeMatrix( this->matrix, this->position, this->quaternion, this->scale );
    }
}