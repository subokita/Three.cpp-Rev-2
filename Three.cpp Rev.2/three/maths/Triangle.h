//
//  Triangle.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Triangle__
#define __Three_cpp_Rev_2__Triangle__

#include <iostream>
#include "internal_headers.h"

namespace three {
    class Triangle {
    public:
        static ptr<Triangle> create();
        static ptr<Triangle> create(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        
        Triangle();
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        ~Triangle();
        
        glm::vec3 normal();
        glm::vec3 midpoint();
        glm::vec3 baryCoordFrom( glm::vec3 point );
        bool contains( glm::vec3 point );
        void setFrom( std::vector<glm::vec3> points, int i0, int i1, int i2 );
        float area();
        ptr<Plane> plane();
        bool equals( const Triangle& other );
        
        
    public:
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Triangle__) */
