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
        static ptr<SphereGeometry> create(const int slices          = 8,
                                          const int parallels       = 6,
                                          const float radius        = 1.0,
                                          const float phi_start     = 0.0,
                                          const float phi_end       = 360.0,
                                          const float theta_start   = 0.0,
                                          const float theta_end     = 180.0);
        ~SphereGeometry();
        
    protected:
        int slices;
        int parallels;
        float radius;
        float phiStart;
        float phiEnd;
        float thetaStart;
        float thetaEnd;
        
        SphereGeometry(const int slices, const int parallels, const float radius,
                       const float phi_start, const float phi_end, const float theta_start, const float theta_end);
        SphereGeometry();
        
        virtual void init() override;
    };
}

#endif /* defined(__Three_cpp_Rev_2__SphereGeometry__) */
