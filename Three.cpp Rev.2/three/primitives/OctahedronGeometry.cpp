//
//  OctahedronGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "OctahedronGeometry.h"

using std::vector;

namespace three {
    
    ptr<OctahedronGeometry> OctahedronGeometry::create(float radius, int detail){
        return make_shared<OctahedronGeometry>(OctahedronGeometry(radius, detail));
    }
    
    OctahedronGeometry::OctahedronGeometry(float radius, int detail):
        PolyhedronGeometry( {glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0),
                             glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, -1.0, 0.0),
                             glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0)},
                            {0, 2, 4, 0, 4, 3,
                             0, 3, 5, 0, 5, 2,
                             1, 2, 5, 1, 5, 3,
                             1, 3, 4, 1, 4, 2},
                            radius, detail )
    {}
    
    OctahedronGeometry::~OctahedronGeometry(){
    }
    
}