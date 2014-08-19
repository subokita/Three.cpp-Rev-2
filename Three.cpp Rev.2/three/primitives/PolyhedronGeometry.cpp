//
//  PolyhedronGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PolyhedronGeometry.h"

using std::vector;

namespace three {
    struct TempVec{
        glm::vec3 vertex;
        glm::vec2 uv;
        int index;
    };
    
    
    PolyhedronGeometry::PolyhedronGeometry( vector<glm::vec3>& vertices, vector<int>& indices, float radius, float detail ) :
        radius( radius ),
        detail( detail )
    {
        
    }
    
    PolyhedronGeometry::~PolyhedronGeometry() {
        
    }
    
    glm::vec3 PolyhedronGeometry::prepare( glm::vec3& vector ) {
        glm::vec3 vertex = glm::normalize( vector );
        
        return vertex;
    }
}