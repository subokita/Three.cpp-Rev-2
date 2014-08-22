//
//  Raycaster.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Raycaster.h"
#include "Ray.h"
#include "Sphere.h"
#include "Mesh.h"

namespace three {
    
    ptr<Raycaster> Raycaster::create(glm::vec3 origin, glm::vec3 direction, float near, float far) {
        return make_shared<Raycaster>(Raycaster(origin, direction, near, far));
    }
    
    Raycaster::Raycaster(glm::vec3 origin, glm::vec3 direction, float near, float far) :
        ray ( Ray::create(origin, direction) ),
        near( near ),
        far ( far )
    {}
    
    Raycaster::Raycaster() :
        near(0.0),
        far (MAX_FLOAT)
    {}
    
    Raycaster::~Raycaster() {
    }
    
    
    bool Raycaster::intersects( ptr<Object3D> object ) {
        if( instance_of(object, Mesh) ) {
            auto mesh = downcast(object, Mesh);
            ptr<Sphere> bounding_sphere = mesh->computeBoundingSphere();
            
            return this->ray->intersects(bounding_sphere);
        }
        return false;
    }
}
