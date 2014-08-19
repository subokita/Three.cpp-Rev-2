//
//  IcosahedronGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "IcosahedronGeometry.h"

using std::vector;
using glm::vec3;

namespace three {
    static const float t = (1 + sqrtf(5.0)) / 2.0;
    
    ptr<IcosahedronGeometry> IcosahedronGeometry::create(float radius, int detail){
        return make_shared<IcosahedronGeometry>(IcosahedronGeometry(radius, detail));
    }
    
    IcosahedronGeometry::IcosahedronGeometry(float radius, int detail):
    PolyhedronGeometry( {vec3(-1.0, t, 0.0), vec3(1.0, t, 0.0), vec3(-1.0, -t, 0.0), vec3(1.0, -t, 0.0),
                         vec3(0.0, -1.0, t), vec3(0.0, 1.0, t), vec3(0.0, -1.0, -t), vec3(0.0, 1.0, -t),
                         vec3(t, 0.0, -1.0), vec3(t, 0.0, 1.0), vec3(-t, 0.0, -1.0), vec3(-t, 0.0, 1.0)},
                       { 0, 11,  5,    0,  5,  1,    0,  1,  7,    0,  7, 10,    0, 10, 11,
                         1,  5,  9,    5, 11,  4,   11, 10,  2,   10,  7,  6,    7,  1,  8,
                         3,  9,  4,    3,  4,  2,    3,  2,  6,    3,  6,  8,    3,  8,  9,
                         4,  9,  5,    2,  4, 11,    6,  2, 10,    8,  6,  7,    9,  8,  1 },
                       radius, detail )
    {}
    
    IcosahedronGeometry::~IcosahedronGeometry(){
    }
    
}