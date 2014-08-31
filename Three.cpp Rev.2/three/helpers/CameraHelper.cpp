//
//  CameraHelper.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "CameraHelper.h"
#include "CubeGeometry.h"
#include "TetrahedronGeometry.h"
#include "LineBasicMaterial.h"
#include "Face3.h"

using std::vector;

namespace three {
    
    ptr<CameraHelper> CameraHelper::create(Color line_color) {
        return make_shared<CameraHelper>(CameraHelper(line_color));
    }
    
    ptr<CameraHelper> CameraHelper::create(glm::vec3 eye, glm::vec3 center, Color line_color) {
        return make_shared<CameraHelper>(CameraHelper(eye, center, line_color));
    }
    
    CameraHelper::CameraHelper(Color line_color) {
        init(line_color);
    }
    
    CameraHelper::CameraHelper(glm::vec3 eye, glm::vec3 center,Color line_color) {
        init(line_color);

        align(eye, center);
    }
    
    CameraHelper::~CameraHelper() {
        
    }
    
    
    void CameraHelper::align(glm::vec3 eye, glm::vec3 center) {
        /* Align the camera so that it's positioned and looking at the correct direction */
        this->position = eye;
        glm::vec3 v1 = glm::vec3(0.0, 0.0, 1.0);
        glm::vec3 v2 = center - eye;
        if( glm::length(v2) != 0.0 )
            v2 = glm::normalize(v2);
        
        float angle = glm::degrees(acosf(glm::dot(v1, v2)));
        if( angle != 0.0 )
            this->rotate(glm::cross(v1, v2), angle );
    }
    
    void CameraHelper::init(Color color) {
        this->material = BasicMaterial::create(color);
        this->material->setOpacity      ( 0.25f );
        this->material->setSide         ( SIDE::DOUBLE_SIDE );
        this->material->setBlendingMode ( BLENDING_MODE::NORMAL );
        
        this->geometry = Geometry::create();
        
        ptr<CubeGeometry> cube = CubeGeometry::create(0.5, 0.5, 1.0);
        vector<glm::vec3> cube_vertices = cube->getVertices();
        vector<ptr<Face3>> cube_faces   = cube->getFaces();
        
        this->geometry->addVertices ( cube_vertices );
        this->geometry->addFaces    ( cube_faces );
        
        int face_offset = static_cast<int>(cube_vertices.size());
        
        this->geometry->addVertex( glm::vec3(0.0, 0.0,  0.50) );
        this->geometry->addVertex( glm::vec3(-0.25, -0.25,  0.75) );
        this->geometry->addVertex( glm::vec3(-0.25,  0.25,  0.75) );
        this->geometry->addVertex( glm::vec3( 0.25, -0.25,  0.75) );
        this->geometry->addVertex( glm::vec3( 0.25,  0.25,  0.75) );
        
        this->geometry->addFace( Face3::create(face_offset, face_offset+1, face_offset+2) );
        this->geometry->addFace( Face3::create(face_offset, face_offset+1, face_offset+3) );
        this->geometry->addFace( Face3::create(face_offset, face_offset+3, face_offset+4) );
        this->geometry->addFace( Face3::create(face_offset, face_offset+2, face_offset+4) );
    }
}