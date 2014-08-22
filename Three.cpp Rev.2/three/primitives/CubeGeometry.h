//
//  CubeGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__CubeGeometry__
#define __Three_cpp_Rev_2__CubeGeometry__

#include <iostream>
#include "Geometry.h"


namespace three {
    class CubeGeometry: public Geometry {
    public:
        static ptr<CubeGeometry> create( glm::vec3 size,
                                         const int width_segments  = 1,
                                         const int height_segments = 1,
                                         const int depth_segments  = 1 );
        
        static ptr<CubeGeometry> create( const float size, const int segments = 1 );
        static ptr<CubeGeometry> create( const float width, const float height, const float depth,
                                         const int width_segments  = 1,
                                         const int height_segments = 1,
                                         const int depth_segments  = 1 );
        ~CubeGeometry();
        
    protected:
        float width;
        float height;
        float depth;
        int widthSegments;
        int heightSegments;
        int depthSegments;
        
        CubeGeometry();
        CubeGeometry( const float size, const int segments );
        CubeGeometry( const float width, const float height, const float depth,
                      const int width_segments, const int height_segments, const int depth_segments);
        
        virtual void init() override;
        
        void buildPlane( int u, int v, int udir, int vdir, float width, float height, float depth, int material_index );
    };
}

#endif /* defined(__Three_cpp_Rev_2__CubeGeometry__) */
