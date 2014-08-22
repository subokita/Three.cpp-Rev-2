//
//  CylinderGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 18/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__CylinderGeometry__
#define __Three_cpp_Rev_2__CylinderGeometry__

#include <iostream>

#include "Geometry.h"

namespace three {
    class CylinderGeometry : public Geometry {
    public:
        static ptr<CylinderGeometry> create(const float radius_top    = 0.5,
                                            const float radius_bottom = 0.5,
                                            const float height        = 1.0,
                                            const int radial_segments = 8,
                                            const int height_segments = 1,
                                            const bool open_ended     = false);
        
        ~CylinderGeometry();
        
    protected:
        CylinderGeometry();
        CylinderGeometry(const float radius_top, const float radius_bottom, const float height,
                         const int radial_segments, const int height_segments, const bool open_ended);
        
        virtual void init() override;
        
        float radiusTop;
        float radiusBottom;
        float height;
        int radialSegments;
        int heightSegments;
        bool openEnded;
    };
}

#endif /* defined(__Three_cpp_Rev_2__CylinderGeometry__) */
