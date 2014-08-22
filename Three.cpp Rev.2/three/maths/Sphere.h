//
//  Sphere.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Sphere__
#define __Three_cpp_Rev_2__Sphere__

#include <iostream>
#include "internal_headers.h"
#include "Utils.h"

namespace three {
    class Sphere {
    public:
        static ptr<Sphere> create();
        static ptr<Sphere> create( glm::vec3 center, float radius );
        
        Sphere();
        Sphere( glm::vec3 center, float radius );
        ~Sphere();
        
        Sphere& set( glm::vec3 center, float radius );
        Sphere& setFrom( std::vector<glm::vec3>& points );
        Sphere& setFrom( std::vector<glm::vec3>& points, glm::vec3 center );
        Sphere& setFrom( ptr<Object3D> obj );
        
        bool empty();
        bool contains( glm::vec3 point );
        float distanceTo( glm::vec3 point );
        bool doesIntersect( Sphere& sphere );
        glm::vec3 clamp( glm::vec3 point );
        Box3 getBoundingBox();
        
        Sphere& applyMatrix( glm::mat4x4& mat );
        
        Sphere& translate( glm::vec3 offset );
        bool equals( Sphere& rhs );
        
    public:
        glm::vec3 center;
        float radius;
        
        friend std::ostream &operator <<( std::ostream& os, const Sphere& sphere ) {
            os << Utils::toString( sphere.center ) << " radius: " << sphere.radius;
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Sphere__) */
