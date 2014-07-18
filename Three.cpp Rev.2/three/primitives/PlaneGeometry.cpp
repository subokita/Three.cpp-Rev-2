//
//  PlaneGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "PlaneGeometry.h"

using namespace std;

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
        
        int gridX = widthSegments;
        int gridZ = heightSegments;
        
        int gridX1 = gridX + 1;
        int gridZ1 = gridZ + 1;
        
        float segment_width = width / gridX;
        float segment_height = height / gridZ;
        
        glm::vec3 normal(0, 0, 1);
        for ( int iz = 0; iz < gridZ1; iz ++ ) {
            float y = iz * segment_height - height_half;
            
            for ( int ix = 0; ix < gridX1; ix ++ ) {
                float x = ix * segment_width - width_half;
                vertices.push_back( glm::vec3(x, -y, 0.0) );
            }
        }
        
        for ( int iz = 0; iz < gridZ; iz ++ ) {
            for ( int ix = 0; ix < gridX; ix ++ ) {
                int a = ix + gridX1 * iz;
                int b = ix + gridX1 * ( iz + 1 );
                int c = ( ix + 1 ) + gridX1 * ( iz + 1 );
                int d = ( ix + 1 ) + gridX1 * iz;
                
                glm::vec2 uva( ix / gridX, 1 - iz / gridZ );
                glm::vec2 uvb( ix / gridX, 1 - ( iz + 1 ) / gridZ );
                glm::vec2 uvc( ( ix + 1 ) / gridX, 1 - ( iz + 1 ) / gridZ );
                glm::vec2 uvd( ( ix + 1 ) / gridX, 1 - iz / gridZ );
                
                ptr<Face3> face = Face3::create( a, b, d, normal );
                face->addVertexNormals({normal, normal, normal});
                face->addVertexUVs({uva, uvb, uvd});
                faces.push_back( face );
                
                face = Face3::create( b, c, d, normal );
                face->addVertexNormals({normal, normal, normal});
                face->addVertexUVs({uvb, uvc, uvd});
                faces.push_back( face );
            }
        }
        
        mergeVertices();
    }
}