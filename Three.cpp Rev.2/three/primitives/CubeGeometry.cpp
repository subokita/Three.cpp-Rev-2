//
//  CubeGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CubeGeometry.h"
#include "Face3.h"
#include <exception>

using std::vector;

namespace three {
    
    CubeGeometry::CubeGeometry():
        width           ( 1.0 ),
        height          ( 1.0 ),
        depth           ( 1.0 ),
        widthSegments   ( 1 ),
        heightSegments  ( 1 ),
        depthSegments   ( 1 )
    {}
    
    CubeGeometry::CubeGeometry(const float size, const int segments ) :
        width           ( size ),
        height          ( size ),
        depth           ( size ),
        widthSegments   ( segments ),
        heightSegments  ( segments ),
        depthSegments   ( segments )
    {
        init();
    }
    
    CubeGeometry::CubeGeometry( const float width, const float height, const float depth,
                               const int width_segments, const int height_segments, const int depth_segments ) :
        width           ( width ),
        height          ( height ),
        depth           ( depth ),
        widthSegments   ( width_segments ),
        heightSegments  ( height_segments ),
        depthSegments   ( depth_segments )
    {
        init();
    }
    
    CubeGeometry::~CubeGeometry(){}
    
    ptr<CubeGeometry> CubeGeometry::create(glm::vec3 size, const int width_segments, const int height_segments,
                                           const int depth_segments) {
        return make_shared<CubeGeometry>(CubeGeometry( size.x, size.y, size.z, width_segments,
                                                       height_segments, depth_segments ));
    }
    
    ptr<CubeGeometry> CubeGeometry::create( const float size, const int segments ) {
        return make_shared<CubeGeometry>(CubeGeometry(size, segments));
    }
    
    
    ptr<CubeGeometry> CubeGeometry::create( const float width, const float height, const float depth,
                                            const int width_segments, const int height_segments, const int depth_segments ) {
        return make_shared<CubeGeometry>(CubeGeometry(width, height, depth, width_segments, height_segments, depth_segments));
    }
    
    
    void CubeGeometry::init() {
        float width_half  = this->width  / 2.0;
        float height_half = this->height / 2.0;
        float depth_half  = this->depth  / 2.0;
        
        buildPlane( 2, 1, - 1, - 1, depth, height,  width_half,     0 ); // px
        buildPlane( 2, 1,   1, - 1, depth, height,  -width_half,    1 ); // nx
        buildPlane( 0, 2,   1,   1, width, depth,   height_half,    2 ); // py
        buildPlane( 0, 2,   1, - 1, width, depth,   -height_half,   3 ); // ny
        buildPlane( 0, 1,   1, - 1, width, height,  depth_half,     4 ); // pz
        buildPlane( 0, 1, - 1, - 1, width, height,  -depth_half,    5 ); // nz
        
//        mergeVertices();
    }
    
    void CubeGeometry::buildPlane( int u, int v, int udir, int vdir,
                                   float width, float height, float depth, int material_index ) {
        
        int w;
        float grid_x = widthSegments;
        float grid_y = heightSegments;
        float width_half  = width  * 0.5;
        float height_half = height * 0.5;
        
		int offset = (int) vertices.size();
        
        glm::vec3 normal;
        
        if( u + v == 1 ){
            w = 2;
        }
        else if( u + v == 2 ) {
            w = 1;
            grid_y = depthSegments;
        }
        else if( u + v == 3 ){
            w = 0;
            grid_x = depthSegments;
        }
        
        normal[w] = depth > 0.0 ? 1.0 : -1.0;
        
        float grid_x1 = grid_x + 1;
		float grid_y1 = grid_y + 1;
		float segment_width = width / grid_x;
		float segment_height = height / grid_y;

        
		for ( float iy = 0; iy < grid_y1; iy += 1.0 ) {
			for ( float ix = 0; ix < grid_x1; ix += 1.0 ) {
                glm::vec3 vector;
				vector[u] = ( ix * segment_width - width_half ) * udir;
				vector[v] = ( iy * segment_height - height_half ) * vdir;
				vector[w] = depth;
				vertices.push_back( vector );
			}
		}
        
        
        for ( float iy = 0; iy < grid_y; iy+=1.0 ) {
            
			for ( float ix = 0; ix < grid_x; ix+=1.0 ) {
				int  a = ix + grid_x1 * iy;
				int  b = ix + grid_x1 * ( iy + 1.0 );
				int  c = ( ix + 1.0 ) + grid_x1 * ( iy + 1.0 );
				int  d = ( ix + 1.0 ) + grid_x1 * iy;
                
                glm::vec2 uva = glm::vec2( ix / grid_x, 1.0 - iy / grid_y );
				glm::vec2 uvb = glm::vec2( ix / grid_x, 1.0 - ( iy + 1.0 ) / grid_y );
				glm::vec2 uvc = glm::vec2( ( ix + 1.0 ) / grid_x, 1.0 - ( iy + 1.0 ) / grid_y );
				glm::vec2 uvd = glm::vec2( ( ix + 1.0 ) / grid_x, 1.0 - iy / grid_y );
                
				ptr<Face3> face = Face3::create( a + offset, b + offset, d + offset );
				face->setFaceNormal( normal );
                face->setVertexNormals( {normal, normal, normal});
                face->setVertexUVs({uva, uvb, uvd});
				faces.push_back( face );
                
				face = Face3::create( b + offset, c + offset, d + offset );
				face->setFaceNormal( normal );
                face->setVertexNormals( {normal, normal, normal});
                face->setVertexUVs({uvb, uvc, uvd});
				faces.push_back( face );
			}
		}
        
        computeVertexNormalsForNonMergedFaces();
    }
}