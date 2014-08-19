//
//  Face3.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Face3.h"

using std::vector;

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
        materialIndex( 0 ),
        vertexColors { glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0) }
    {}
    
    
    Face3::Face3( int a, int b, int c ):
        a            ( a ),
        b            ( b ),
        c            ( c ),
        materialIndex( 0 ),
        vertexColors { glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0) }
    {}
    
    Face3::Face3( int a, int b, int c, glm::vec3 normal, Color color, int material_index ):
        a            ( a ),
        b            ( b ),
        c            ( c ),
        normal       ( normal ),
        color        ( color ),
        materialIndex( material_index ),
        vertexColors { glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0) }
    {}
    
    Face3::~Face3(){}
    
    
    const glm::vec3& Face3::getFaceNormal() {
        return this->normal;
    }
    
    void Face3::setFaceNormal(glm::vec3 normal){
        this->normal = normal;
    }
    void Face3::setFaceNormal(float x, float y, float z) {
        this->normal.x = x;
        this->normal.y = y;
        this->normal.z = z;
    }
    
    void Face3::setVertexColors( std::initializer_list<glm::vec3> vertex_colors ) {
        vertexColors[0] = (*(vertex_colors.begin()  ));
        vertexColors[1] = (*(vertex_colors.begin()+1));
        vertexColors[2] = (*(vertex_colors.begin()+2));
    }
    
    void Face3::setVertexColors( std::initializer_list<Color> vertex_colors ) {
        vertexColors[0] = (*(vertex_colors.begin()  )).rep;
        vertexColors[1] = (*(vertex_colors.begin()+1)).rep;
        vertexColors[2] = (*(vertex_colors.begin()+2)).rep;
    }
    
    void Face3::setVertexNormals( std::initializer_list<glm::vec3> vertex_normals ) {
        vertexNormals[0] = *(vertex_normals.begin()  );
        vertexNormals[1] = *(vertex_normals.begin()+1);
        vertexNormals[2] = *(vertex_normals.begin()+2);
    }
    
    void Face3::setVertexUVs( std::initializer_list<glm::vec2> vertex_uvs ) {
        uvs[0] = *(vertex_uvs.begin()  );
        uvs[1] = *(vertex_uvs.begin()+1);
        uvs[2] = *(vertex_uvs.begin()+2);
    }
}