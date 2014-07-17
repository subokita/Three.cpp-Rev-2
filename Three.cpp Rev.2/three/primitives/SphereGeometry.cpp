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
    
    PTR(SphereGeometry) SphereGeometry::create(const unsigned int slices, const unsigned int parallels, const float size){
        return make_shared<SphereGeometry>( SphereGeometry(slices, parallels, size) );
    }
    

    SphereGeometry::SphereGeometry(const unsigned int slices, const unsigned int parallels, const float size){
        this->init(slices, parallels, size );
    }
    
    SphereGeometry::SphereGeometry(){
    }
    
    SphereGeometry::~SphereGeometry(){
    }
    
    
    void SphereGeometry::init(const unsigned int slices, const unsigned int parallels, const float size){
        if( size <= 0.0f )
            throw std::runtime_error( "Unable to create sphere with size less than 0.0" );
        
        float angle_step = (M_PI * 2.0) / slices ;
        vector<glm::vec2> uvs;
        
        for( int i = 0; i < parallels + 1; i++ ) {
            for( int j = 0; j < slices + 1; j++ ) {
                glm::vec3 vertex;
                vertex.x = sinf( angle_step * i ) * sinf( angle_step * j );
                vertex.y = cosf( angle_step * i );
                vertex.z = sinf( angle_step * i ) * cosf( angle_step * j );
                
                glm::vec2 uv( j * 1.0 / slices, 1.0 - i * 1.0 / parallels );
                
                uvs.push_back( uv );
                vertices.push_back( vertex );
            }
        }
        
        for( glm::vec3& vert: vertices )
            vert *= size;
        
        vector<unsigned short> index( slices * parallels * 6 );
        int index_counter = 0;
        for( int i = 0; i < parallels; i++ ) {
            for( int j = 0; j < slices; j++ ) {
                index[index_counter++] = i       * (slices + 1) + j;
                index[index_counter++] = (i + 1) * (slices + 1) + j;
                index[index_counter++] = (i + 1) * (slices + 1) + (j + 1);
                
                index[index_counter++] = i       * (slices + 1) + j;
                index[index_counter++] = (i + 1) * (slices + 1) + (j + 1);
                index[index_counter++] = i       * (slices + 1) + (j + 1);
            }
        }
        
        for( int i = 0; i < index.size(); i+= 3 ) {
            PTR(Face3) face = Face3::create(index[i], index[i+1], index[i+2]);
            
            face->uvs.push_back( uvs[i] );
            face->uvs.push_back( uvs[i+1] );
            face->uvs.push_back( uvs[i+2] );
            faces.push_back( face );
        }
        
        mergeVertices();
        computeFaceNormals();
        computeVertexNormals(true);
    }
}