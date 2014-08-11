//
//  Projector.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 26/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Projector__
#define __Three_cpp_Rev_2__Projector__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class Projector {
    public:
        static glm::vec4 projectVector( const glm::vec4 vector, ptr<Camera> camera );
        static glm::vec4 unprojectVector( const glm::vec4 vector, ptr<Camera> camera );
        
        static ptr<Raycaster> pickingRay( const glm::vec3 normalized_screen_coord, ptr<Camera> camera );
    };
}

#endif /* defined(__Three_cpp_Rev_2__Projector__) */
