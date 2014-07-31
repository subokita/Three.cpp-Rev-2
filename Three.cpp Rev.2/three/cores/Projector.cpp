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
        glm::mat4 view_projection_mat = camera->projection * camera->matrixWorldInverse;
        return glm::vec3(glm::vec4(vector, 1.0) * view_projection_mat);
    }
    
    glm::vec3 Projector::unprojectVector( glm::vec3 vector, ptr<Camera> camera ){
        glm::mat4 projection_inverse = glm::inverse( camera->projection );
        glm::mat4 view_projection_mat = camera->matrixWorld * projection_inverse;
        return glm::vec3(glm::vec4(vector, 1.0) * view_projection_mat);
    }
}