//
//  OctahedronGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__OctahedronGeometry__
#define __Three_cpp_Rev_2__OctahedronGeometry__

#include <iostream>
#include "Geometry.h"
#include "PolyhedronGeometry.h"

namespace three {
    class OctahedronGeometry : public PolyhedronGeometry {
    public:
        static ptr<OctahedronGeometry> create(float radius = 1.0, int detail = 0);
        
        ~OctahedronGeometry();
        
    protected:
        OctahedronGeometry(float radius, int detail);
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__OctahedronGeometry__) */
