//
//  PolyhedronGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PolyhedronGeometry__
#define __Three_cpp_Rev_2__PolyhedronGeometry__

#include <iostream>
#include "Geometry.h"

namespace three {
    class PolyhedronGeometry: public Geometry{
    public:
        PolyhedronGeometry( std::vector<glm::vec3>& vertices, std::vector<int>& indices, float radius, float detail );
        ~PolyhedronGeometry();
        
    protected:
        glm::vec3 prepare( glm::vec3& vector );
        
    protected:
        float radius;
        float detail;
        
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__PolyhedronGeometry__) */
