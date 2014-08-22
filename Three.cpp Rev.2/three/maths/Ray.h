//
//  Ray.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Ray__
#define __Three_cpp_Rev_2__Ray__

#include <iostream>
#include "internal_headers.h"
#include "Utils.h"

namespace three {
    class Ray {
    public:
        static ptr<Ray> create( glm::vec3 origin, glm::vec3 direction );
        Ray();
        Ray( glm::vec3 origin, glm::vec3 direction );
        
        ~Ray();
        
        void set( glm::vec3 origin, glm::vec3 direction );
        
        glm::vec3 at(float t);
        glm::vec3 closestPointTo( glm::vec3 point );
        
        float distanceTo( glm::vec3 point );
        float distanceSquaredTo( glm::vec3 v0, glm::vec3 v1 );
        float distanceTo( ptr<Plane> plane );
        
        bool intersects( ptr<Sphere> sphere );
        bool intersects( ptr<Plane> plane );
        bool intersects( ptr<Box3> box );
        
        glm::vec3 intersectsPlaneAt( ptr<Plane> plane );
        glm::vec3 intersectsBoxAt( ptr<Box3> box );
        
        void applyMatrix( glm::mat4& mat );
        bool equals(ptr<Ray> other);
        
        glm::vec3 intersectsTriangleAt( ptr<Triangle> triangle, bool backface_cull = true );
        
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Ray& obj ) {
            os << "Ray {" << std::endl;
            os << "\torigin   : " << Utils::toString( obj.origin ) << std::endl;
            os << "\tdirection: " << Utils::toString( obj.direction ) << std::endl;
            os << "}";
            return os;
        }
        
    protected:
        glm::vec3 origin;
        glm::vec3 direction;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Ray__) */
