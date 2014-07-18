//
//  Face3.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Face3.h"

using namespace std;

namespace  three {
    
    
    ptr<Face3> Face3::create(int a, int b, int c) {
        return make_shared<Face3>(Face3(a, b, c));
    }
    
    ptr<Face3> Face3::create(int a, int b, int c, glm::vec3 normal, Color color, int material_index) {
        return make_shared<Face3>(Face3(a, b, c, normal, color, material_index));
    }
    
    Face3::Face3() :
        a            ( 0 ),
        b            ( 0 ),
        c            ( 0 ),
        materialIndex( 0 )
    {}
    
    
    Face3::Face3( int a, int b, int c ):
        a            ( a ),
        b            ( b ),
        c            ( c ),
        materialIndex( 0 )
    {}
    
    Face3::Face3( int a, int b, int c, glm::vec3 normal, Color color, int material_index ):
        a            ( a ),
        b            ( b ),
        c            ( c ),
        normal       ( normal ),
        color        ( color ),
        materialIndex( material_index )
    {}
    
    Face3::~Face3(){}
    
    
    void Face3::addVertexNormals( std::initializer_list<glm::vec3> vertex_normals ) {
        for( glm::vec3 normal: vertex_normals )
            this->vertexNormals.push_back( normal );
    }
    
    void Face3::addVertexUVs( std::initializer_list<glm::vec2> vertex_uvs ) {
        for( glm::vec2 uv: vertex_uvs )
            this->uvs.push_back( uv );
    }
}