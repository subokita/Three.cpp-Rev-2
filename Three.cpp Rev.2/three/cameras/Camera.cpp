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
    Camera::Camera():
        projection(glm::mat4(1.0))
    {}
    
    Camera::Camera(const glm::vec3 position, const glm::vec3 up):
        projection(glm::mat4(1.0))
    {
        this->position  = position;
        this->up        = up;
    }
    
    Camera::~Camera(){}
    
    const glm::mat4& Camera::getProjectionMatrix() {
        return this->projection;
    }
    
    void Camera::lookAt( const float x, const float y, const float z ) {
        this->lookAt(glm::vec3(x, y, z));
    }
    
    
    void Camera::updateMatrix() {
        this->matrix = glm::lookAt( this->position, this->target->position, this->up );
    }
    
    
    void Camera::lookAt(const glm::vec3 vec ) {
        this->setTarget( vec );
        this->matrix = glm::lookAt( this->position, vec, this->up );
        
//        glm::vec3 temp = this->position;
//        Math::decomposeMatrix( this->matrix, this->position, this->quaternion, this->scale );
//        std::swap(temp, position);
        
        this->quaternion = glm::quat_cast(this->matrix);
    }
}