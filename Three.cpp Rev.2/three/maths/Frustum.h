//
//  Frustum.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 15/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Frustum__
#define __Three_cpp_Rev_2__Frustum__

#include <iostream>
#include <vector>
#include "internal_headers.h"

namespace three {
    class Frustum{
    public:
        static ptr<Frustum> create();
        
        Frustum();
        ~Frustum();
        
        Frustum& setFrom( glm::mat4& matrix );
        bool intersects( ptr<Object3D> object );
        bool intersects( ptr<Sphere> sphere );
        bool intersects( ptr<Box3> box );
        bool contains( glm::vec3 point );
        
    public:
        std::vector<ptr<Plane>> planes;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Frustum__) */
