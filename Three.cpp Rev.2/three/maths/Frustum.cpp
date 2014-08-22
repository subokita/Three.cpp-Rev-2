//
//  Frustum.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 15/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Frustum.h"
#include "Box3.h"
#include "Plane.h"
#include "Sphere.h"
#include "Object3D.h"

namespace three {
    
    ptr<Frustum> Frustum::create(){
        return make_shared<Frustum>();
    }
    
    Frustum::Frustum(){
        planes.resize(6);
        for( ptr<Plane>& plane: planes )
            plane = Plane::create();
    }
    
    Frustum::~Frustum(){}
    
    Frustum& Frustum::setFrom( glm::mat4& m ){
        planes[0]->set( m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]).normalize();
        planes[1]->set( m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]).normalize();
        
        planes[2]->set( m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]).normalize();
        planes[3]->set( m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]).normalize();
        
        planes[4]->set( m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]).normalize();
        planes[5]->set( m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]).normalize();
        
        return *this;
    }
    
    bool Frustum::intersects( ptr<Object3D> object ){
        ptr<Sphere> sphere = object->computeBoundingSphere();
        sphere->applyMatrix( object->matrixWorld );
        return intersects( sphere );
    }
    
    bool Frustum::intersects( ptr<Sphere> sphere ){
        for( int i = 0; i < 6; i++ ) {
            float distance = planes[i]->distanceTo( sphere->center );
            if( distance < -sphere->radius )
                return false;
        }
        return true;
    }
    
    bool Frustum::intersects( ptr<Box3> box ){
        glm::vec3 p1, p2;
        
        for( int i = 0; i < 6; i++ ) {
            p1.x = planes[i]->normal.x > 0 ? box->min.x : box->max.x;
            p2.x = planes[i]->normal.x > 0 ? box->max.x : box->min.x;
            
            p1.y = planes[i]->normal.y > 0 ? box->min.y : box->max.y;
            p2.y = planes[i]->normal.y > 0 ? box->max.y : box->min.y;
            
            p1.z = planes[i]->normal.z > 0 ? box->min.z : box->max.z;
            p2.z = planes[i]->normal.z > 0 ? box->max.z : box->min.z;
            
            float d1 = planes[i]->distanceTo( p1 );
            float d2 = planes[i]->distanceTo( p2 );
            
            if( d1 < 0.0 && d2 < 0.0 )
                return false;
        }
        
        return true;
    }
    
    bool Frustum::contains( glm::vec3 point ){
        for( int i = 0; i < 6; i++ ) {
            if( planes[i]->distanceTo( point ) < 0.0 )
                return false;
        }
        return true;
    }
}