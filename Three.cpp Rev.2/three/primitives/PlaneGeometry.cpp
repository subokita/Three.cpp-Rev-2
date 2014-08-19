//
//  PlaneGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PlaneGeometry.h"
#include "Face3.h"

using std::vector;

namespace three {
    
    ptr<PlaneGeometry> PlaneGeometry::create( const float size, const int segments ) {
        return std::make_shared<PlaneGeometry>(PlaneGeometry(size, segments));
    }
    
    PlaneGeometry::PlaneGeometry() :
        width         ( 1.0 ),
        height        ( 1.0 ),
        widthSegments ( 1 ),
        heightSegments( 1 )
    {
        
    }
    
    PlaneGeometry::PlaneGeometry(const float size, const int segments):
        width         ( size ),
        height        ( size ),
        widthSegments ( segments ),
        heightSegments( segments )
    
    {
        init();
    }
    
    PlaneGeometry::~PlaneGeometry(){
        
    }
    
    void PlaneGeometry::init() {
        
        float width_half  = width / 2.0;
        float height_half = height / 2.0;
        
        float gridX = widthSegments;
        float gridZ = heightSegments;
        
        float gridX1 = gridX + 1.0;
        float gridZ1 = gridZ + 1.0;
        
        float segment_width = width / gridX;
        float segment_height = height / gridZ;
        
        glm::vec3 normal(0, 0, 1);
        for ( float iz = 0; iz < gridZ1; iz += 1.0 ) {
            float y = iz * segment_height - height_half;
            
            for ( float ix = 0; ix < gridX1; ix += 1.0 ) {
                float x = ix * segment_width - width_half;
                vertices.push_back( glm::vec3(x, -y, 0.0) );
            }
        }
        
        for ( float iz = 0; iz < gridZ; iz += 1.0 ) {
            for ( float ix = 0; ix < gridX; ix +=1.0 ) {
                int a = ix + gridX1 * iz;
                int b = ix + gridX1 * ( iz + 1 );
                int c = ( ix + 1 ) + gridX1 * ( iz + 1 );
                int d = ( ix + 1 ) + gridX1 * iz;
                
                glm::vec2 uva( ix / gridX, 1.0 - iz / gridZ );
                glm::vec2 uvb( ix / gridX, 1.0 - ( iz + 1 ) / gridZ );
                glm::vec2 uvc( ( ix + 1.0 ) / gridX, 1.0 - ( iz + 1.0 ) / gridZ );
                glm::vec2 uvd( ( ix + 1.0 ) / gridX, 1.0 - iz / gridZ );
                
                ptr<Face3> face = Face3::create( a, b, d, normal );
                face->setVertexNormals({normal, normal, normal});
                face->setVertexUVs({uva, uvb, uvd});
                faces.push_back( face );
                
                face = Face3::create( b, c, d, normal );
                face->setVertexNormals({normal, normal, normal});
                face->setVertexUVs({uvb, uvc, uvd});
                faces.push_back( face );
            }
        }
        
//        mergeVertices();
    }
}