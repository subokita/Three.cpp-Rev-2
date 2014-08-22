//
//  Plane.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Plane__
#define __Three_cpp_Rev_2__Plane__

#include <iostream>
#include "Utils.h"
#include "internal_headers.h"


namespace three {
    class Plane {
    public:
        static ptr<Plane> create();
        static ptr<Plane> create( glm::vec3 normal, float constant );
        
        Plane();
        Plane( glm::vec3 normal, float constant );
        ~Plane();
        
        Plane& set( glm::vec3 normal, float constant );
        Plane& set( float x, float y, float z, float w );
        Plane& setFromNormalAndCoplanarPoint( glm::vec3 normal, glm::vec3 point );
        Plane& setFromCoplanarPoints( glm::vec3 a, glm::vec3 b, glm::vec3 c );
        Plane& normalize();
        Plane& negate();
        
        float distanceTo( glm::vec3 point );
        float distanceTo( Sphere& sphere );
        
        glm::vec3 orthoPoint( glm::vec3 point );
        glm::vec3 projectPoint( glm::vec3 point );
        glm::vec3 intersectLine(Line3& line);
        glm::vec3 coplanarPoint();
        
        Plane& applyMatrix( glm::mat4x4& mat );
        Plane& translate(glm::vec3 offset);
        
        bool intersects(Line3& line);
        bool equals(Plane& other );
        
        
    public:
        glm::vec3 normal;
        float constant;
        
        friend std::ostream &operator <<( std::ostream& os, const Plane& plane ) {
            os << Utils::toString( plane.normal ) << " constant: " << plane.constant;
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Plane__) */
