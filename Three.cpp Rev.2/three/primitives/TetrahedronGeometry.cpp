//
//  TetrahedronGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "TetrahedronGeometry.h"

using std::vector;
using glm::vec3;

namespace three {
    
    ptr<TetrahedronGeometry> TetrahedronGeometry::create(float radius, int detail){
        return make_shared<TetrahedronGeometry>(TetrahedronGeometry(radius, detail));
    }
    
    TetrahedronGeometry::TetrahedronGeometry(float radius, int detail):
        PolyhedronGeometry( {vec3( 1.0, 1.0,  1.0), vec3(-1.0, -1.0,  1.0),
                             vec3(-1.0, 1.0, -1.0), vec3( 1.0, -1.0, -1.0)},
                            { 2, 1, 0, 0, 3, 2,
                              1, 3, 0, 2, 3, 1 },
                           radius, detail )
    {}
    
    TetrahedronGeometry::~TetrahedronGeometry(){
    }
    
}