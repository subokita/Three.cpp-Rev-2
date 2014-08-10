//
//  Raycaster.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Raycaster__
#define __Three_cpp_Rev_2__Raycaster__

#include <iostream>
#include "internal_headers.h"


namespace three {
    class Raycaster {
    public:
        static ptr<Raycaster> create(glm::vec3 origin, glm::vec3 direction, float near, float far);

        Raycaster(glm::vec3 origin, glm::vec3 direction, float near, float far);
        Raycaster();
        ~Raycaster();
        
        bool intersects( ptr<Object3D> object );
        
    public:
        ptr<Ray> ray;
        float near;
        float far;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Raycaster__) */
