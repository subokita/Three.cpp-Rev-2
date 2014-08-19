//
//  TorusGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "TorusGeometry.h"
#include "Face3.h"

using std::vector;

namespace three {
    
    ptr<TorusGeometry> TorusGeometry::create(float radius, float tube, int radial_segments, int tubular_segments, float arc){
        return make_shared<TorusGeometry>(TorusGeometry(radius, tube, radial_segments, tubular_segments, arc));
    }
    
    TorusGeometry::TorusGeometry(float radius, float tube, int radial_segments, int tubular_segments, float arc) :
        radius          (radius),
        tube            (tube),
        radialSegments  (radial_segments),
        tubularSegments (tubular_segments),
        arc             (arc)
    {
        this->init();
    }
    
    TorusGeometry::~TorusGeometry(){
    }
    
    void TorusGeometry::init() {
        this->vertices.clear();
        
        vector<glm::vec2> uvs;
        vector<glm::vec3> normals;
        
        float DOUBLE_PI = M_PI * 2.0;
        
        for( float j = 0.0; j <= radialSegments; j++ ) {
            float v = j / radialSegments * DOUBLE_PI;
            float cos_v = cosf(v);
            float sin_v = sinf(v);
            
            for( float i = 0.0; i <= tubularSegments; i++ ) {
                float u = i / tubularSegments * arc;
                float cos_u = cosf(u);
                float sin_u = sinf(u);
                
                glm::vec3 center( radius * cos_u, radius * sin_u, 0.0f );
                glm::vec3 vertex( (radius + tube * cos_v) * cos_u,
                                  (radius + tube * cos_v) * sin_u,
                                                     tube * sin_v );
                
                this->vertices.push_back( vertex );
                uvs.push_back           ( glm::vec2( i / tubularSegments, j / radialSegments ) );
                normals.push_back       ( glm::normalize(vertex - center) );
            }
        }
        
        for( int j = 1; j <= radialSegments; j++ ){
            for( int i = 1; i <= tubularSegments; i++ ){
                int a = (tubularSegments + 1) * j + i - 1;
                int b = (tubularSegments + 1) * (j - 1) + i - 1;
                int c = (tubularSegments + 1) * (j - 1) + i;
                int d = (tubularSegments + 1) * j + i;
                
                ptr<Face3> face = Face3::create(a, b, d);
                face->setVertexNormals  ({ normals[a], normals[b], normals[d] });
                face->setVertexUVs      ({ uvs[a], uvs[b], uvs[d] });
                this->faces.push_back(face);
                
                face = Face3::create(b, c, d);
                face->setVertexNormals  ({ normals[b], normals[c], normals[d] });
                face->setVertexUVs      ({ uvs[b], uvs[c], uvs[d] });
                this->faces.push_back(face);
            }
        }
    }
}