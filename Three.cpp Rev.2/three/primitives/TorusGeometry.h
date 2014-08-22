//
//  TorusGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__TorusGeometry__
#define __Three_cpp_Rev_2__TorusGeometry__

#include <iostream>
#include "Geometry.h"

namespace three {
    class TorusGeometry : public Geometry {
    public:
        static ptr<TorusGeometry> create(float radius = 0.5, float tube = 0.25,
                                         int radial_segments = 40, int tubular_segments = 30, float arc = M_PI * 2.0 );
        
        ~TorusGeometry();
        
    protected:
        TorusGeometry(float radius, float tube, int radial_segments, int tubular_segments, float arc);
        virtual void init() override;
        
    protected:
        float radius;
        float tube;
        int radialSegments;
        int tubularSegments;
        float arc;
        
    };
}


#endif /* defined(__Three_cpp_Rev_2__TorusGeometry__) */
