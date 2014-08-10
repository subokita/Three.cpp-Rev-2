//
//  Projector.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Projector.h"

#include "three.h"

namespace three {
    
    glm::vec3 Projector::projectVector( glm::vec3 vector, ptr<Camera> camera ){
        camera->matrixWorldInverse = glm::inverse( camera->matrixWorld );
        glm::mat4 view_projection_mat = camera->getProjectionMatrix() * camera->matrixWorldInverse;
        return glm::vec3(view_projection_mat * glm::vec4(vector, 0.0));
    }
    
    glm::vec3 Projector::unprojectVector( glm::vec3 vector, ptr<Camera> camera ){
        glm::mat4 inverse_vp = glm::inverse(camera->getProjectionMatrix() * camera->matrix);
        return glm::vec3( inverse_vp * glm::vec4(vector, 0.0) ) + camera->position;
    }
    
    
    ptr<Raycaster> Projector::pickingRay( glm::vec3 vector, ptr<Camera> camera ) {
        vector.z = -1.0;
        glm::vec3 end( vector.x, vector.y, 1.0 );
        
        vector  = unprojectVector(vector, camera );
        end     = unprojectVector(end, camera);
        
        end = glm::normalize(end - vector);
        return Raycaster::create(vector, end, 0.0, MAX_FLOAT);
    }
}