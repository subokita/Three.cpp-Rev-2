//
//  IcosahedronGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__IcosahedronGeometry__
#define __Three_cpp_Rev_2__IcosahedronGeometry__

#include <iostream>

#include <iostream>
#include "Geometry.h"
#include "PolyhedronGeometry.h"

namespace three {
    class IcosahedronGeometry : public PolyhedronGeometry {
    public:
        static ptr<IcosahedronGeometry> create(float radius = 1.0, int detail = 0);
        
        ~IcosahedronGeometry();
        
    protected:
        IcosahedronGeometry(float radius, int detail);
        
    };
}


#endif /* defined(__Three_cpp_Rev_2__IcosahedronGeometry__) */
