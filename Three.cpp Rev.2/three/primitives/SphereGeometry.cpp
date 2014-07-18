//
//  SphereGeometry.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 10/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "SphereGeometry.h"
#include <vector>

using namespace std;
namespace three {
    
    ptr<SphereGeometry> SphereGeometry::create(const int slices, const int parallels, const float size){
        return make_shared<SphereGeometry>( SphereGeometry(slices, parallels, size) );
    }
    

    SphereGeometry::SphereGeometry(const int slices, const int parallels, const float size){
        this->init(slices, parallels, size );
    }
    
    SphereGeometry::SphereGeometry(){
    }
    
    SphereGeometry::~SphereGeometry(){
    }
    
    
    void SphereGeometry::init( int slices, int parallels, const float radius ){
        int width_segments  = max( 3, slices );
        int height_segments = max( 2, parallels );
        
        float phi_start = 0.0;
        float phi_length = M_PI * 2.0;
        
        float theta_start = 0.0;
        float theta_length = M_PI;
        
        this->vertices.clear();
        
        vector2D<uint> face_indexes( height_segments + 1, vector<uint>(width_segments + 1, 0) );
        vector2D<glm::vec2> uvs( height_segments + 1, vector<glm::vec2>(width_segments + 1, glm::vec2(0.0)) );
        
        for( int y = 0; y < height_segments + 1; y++ ) {
            float v = y * 1.0 / height_segments;
            
            float sin_theta = sinf( theta_start + v * theta_length );
            float cos_theta = cosf( theta_start + v * theta_length );
            
            for( int x = 0; x < width_segments + 1; x++ ) {
                float u = x * 1.0 / width_segments;
                
                glm::vec3 vertex(
                    -radius * cosf( phi_start + u * phi_length ) * sin_theta,
                    radius * cos_theta,
                    radius * sinf( phi_start + u * phi_length ) * sin_theta
                );
                
                vertices.push_back( vertex );
                face_indexes[y][x] = static_cast<uint>( this->vertices.size() - 1);
                uvs[y][x] = glm::vec2(u, 1.0 - v);
            }
        }
        
        
        for(int y = 0; y < height_segments; y++ ) {
            for( int x = 0; x < width_segments; x++ ) {
                uint v1 = face_indexes[y  ][x+1];
                uint v2 = face_indexes[y  ][x  ];
                uint v3 = face_indexes[y+1][x  ];
                uint v4 = face_indexes[y+1][x+1];
                
                glm::vec3 n1 = glm::normalize(this->vertices[v1]);
                glm::vec3 n2 = glm::normalize(this->vertices[v2]);
                glm::vec3 n3 = glm::normalize(this->vertices[v3]);
                glm::vec3 n4 = glm::normalize(this->vertices[v4]);
                
                glm::vec2 uv1 = uvs[y  ][x+1];
                glm::vec2 uv2 = uvs[y  ][x  ];
                glm::vec2 uv3 = uvs[y+1][x  ];
                glm::vec2 uv4 = uvs[y+1][x+1];
                
                if( fabs( this->vertices[v1].y ) == radius ) {
                    uv1.x = (uv1.x + uv2.x) / 2.0;
                    
                    ptr<Face3> face = Face3::create( v1, v3, v4 );
                    
                    face->vertexNormals.push_back( n1 );
                    face->vertexNormals.push_back( n3 );
                    face->vertexNormals.push_back( n4 );
                    
                    face->uvs.push_back( uv1 );
                    face->uvs.push_back( uv3 );
                    face->uvs.push_back( uv4 );
                    faces.push_back( face );
                }
                else if( fabs( this->vertices[v3].y ) == radius ) {
                    uv3.x = (uv3.x + uv4.x) / 2.0;
                    
                    ptr<Face3> face = Face3::create( v1, v2, v3 );
                    
                    face->vertexNormals.push_back( n1 );
                    face->vertexNormals.push_back( n2 );
                    face->vertexNormals.push_back( n3 );
                    
                    face->uvs.push_back( uv1 );
                    face->uvs.push_back( uv2 );
                    face->uvs.push_back( uv3 );
                    
                    faces.push_back( face );
                }
                else {
                    ptr<Face3> face = Face3::create( v1, v2, v4 );
                    
                    face->vertexNormals.push_back( n1 );
                    face->vertexNormals.push_back( n2 );
                    face->vertexNormals.push_back( n4 );
                    
                    face->uvs.push_back( uv1 );
                    face->uvs.push_back( uv2 );
                    face->uvs.push_back( uv4 );
                    faces.push_back( face );
                    
                    face = Face3::create( v2, v3, v4 );
                    
                    face->vertexNormals.push_back( n2 );
                    face->vertexNormals.push_back( n3 );
                    face->vertexNormals.push_back( n4 );
                    
                    face->uvs.push_back( uv2 );
                    face->uvs.push_back( uv3 );
                    face->uvs.push_back( uv4 );
                    faces.push_back( face );
                }
            }
        }
        
        computeFaceNormals();
    }
}