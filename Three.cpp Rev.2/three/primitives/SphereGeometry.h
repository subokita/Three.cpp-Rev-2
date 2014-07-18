//
//  SphereGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__SphereGeometry__
#define __Three_cpp_Rev_2__SphereGeometry__

#include <iostream>
#include "Geometry.h"

namespace three {
    class SphereGeometry: public Geometry {
    public:
        static ptr<SphereGeometry> create(const int slices, const int parallels, const float size);
        ~SphereGeometry();
        
    protected:
        SphereGeometry(const int slices, const int parallels, const float size);
        SphereGeometry();
        
        void init(const int slices, const int parallels, const float size);
    };
}

#endif /* defined(__Three_cpp_Rev_2__SphereGeometry__) */
