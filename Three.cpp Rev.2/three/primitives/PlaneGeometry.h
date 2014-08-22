//
//  PlaneGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PlaneGeometry__
#define __Three_cpp_Rev_2__PlaneGeometry__

#include <iostream>
#include "Geometry.h"


namespace three {
    class PlaneGeometry: public Geometry {
    public:
        static ptr<PlaneGeometry> create( const float size, const int segments = 1 );
        ~PlaneGeometry();
        
    protected:
        float width;
        float height;
        int widthSegments;
        int heightSegments;
        
        PlaneGeometry();
        PlaneGeometry(const float size, const int segments);
        virtual void init() override;
        
    };
}

#endif /* defined(__Three_cpp_Rev_2__PlaneGeometry__) */
