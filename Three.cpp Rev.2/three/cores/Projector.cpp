//
//  Projector.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Projector.h"
#include "Raycaster.h"
#include "Camera.h"

namespace three {
    glm::vec4 Projector::projectVector( const glm::vec4 vector, ptr<Camera> camera ){
        glm::mat4 vp = camera->getProjectionMatrix() * camera->matrix;
        return vp * vector;
    }
    
    glm::vec4 Projector::unprojectVector( const glm::vec4 vector, ptr<Camera> camera ){
        glm::mat4 inverse_vp = glm::inverse(camera->getProjectionMatrix() * camera->matrix);
        glm::vec4 result = inverse_vp * vector;
        result /= result.w;
        return result;
    }
    
    
    ptr<Raycaster> Projector::pickingRay( const glm::vec3 normalized_screen_coord, ptr<Camera> camera ) {
        glm::vec4 ray_start(normalized_screen_coord.x, normalized_screen_coord.y, -1.0, 1.0);
        glm::vec4 ray_end  (normalized_screen_coord.x, normalized_screen_coord.y,  0.0, 1.0);

        ray_start = unprojectVector(ray_start, camera );
        ray_end   = unprojectVector(ray_end,   camera );
        
        glm::vec3 origin    = glm::vec3(ray_start);
        glm::vec3 direction = glm::normalize( glm::vec3( ray_end - ray_start ) );
        
        return Raycaster::create(origin, direction, 0.0, MAX_FLOAT);
    }
}