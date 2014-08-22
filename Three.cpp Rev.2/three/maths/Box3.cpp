//
//  Box3.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Box3.h"
#include "Geometry.h"
#include "Object3D.h"
#include "Mesh.h"
#include "Sphere.h"

using std::vector;

namespace three {
    
    ptr<Box3> Box3::create(){
        return make_shared<Box3>();
    }
    
    ptr<Box3> Box3::create( glm::vec3 min_vec, glm::vec3 max_vec ){
        return make_shared<Box3>( Box3(min_vec, max_vec) );
    }
    
    Box3::Box3():
        min( glm::vec3(MAX_FLOAT) ),
        max( glm::vec3(MIN_FLOAT) )
    {}
    
    Box3::Box3( glm::vec3 min_vec, glm::vec3 max_vec ):
        min(min_vec),
        max(max_vec)
    {}
    
    Box3::~Box3(){}
    
    
    void Box3::set( glm::vec3 min_vec, glm::vec3 max_vec ) {
        this->min = min_vec;
        this->max = max_vec;
    }
    
    
    
    void Box3::addPoint( glm::vec3 point ) {
        if( point.x < min.x )
            min.x = point.x;
        else if( point.x > max.x )
            max.x = point.x;
        
        if( point.y < min.y )
            min.y = point.y;
        else if( point.y > max.y )
            max.y = point.y;
        
        if( point.z < min.z )
            min.z = point.z;
        else if( point.z > max.z )
            max.z = point.z;
    }
    
    void Box3::setFrom( vector<glm::vec4>& points ) {
        if( points.empty() ) {
            this->makeEmpty();
        }
        else {
            this->min = glm::vec3(points[0]);
            this->max = glm::vec3(points[0]);
            
            for( size_t i = 1; i < points.size(); i++ ) {
                if( points[i].x < min.x )
                    min.x = points[i].x;
                else if( points[i].x > max.x )
                    max.x = points[i].x;
                
                if( points[i].y < min.y )
                    min.y = points[i].y;
                else if( points[i].y > max.y )
                    max.y = points[i].y;
                
                if( points[i].z < min.z )
                    min.z = points[i].z;
                else if( points[i].z > max.z )
                    max.z = points[i].z;
            }
        }
    }
    
    void Box3::setFrom( vector<glm::vec3>& points ) {
        if( points.empty() ) {
            this->makeEmpty();
        }
        else {
            this->min = points[0];
            this->max = points[0];
            
            for( size_t i = 1; i < points.size(); i++ )
                addPoint( points[i] );
        }
    }
    
    
    void Box3::setFrom( glm::vec3 center, glm::vec3 size ) {
        glm::vec3 half_size = size;
        half_size /= 2.0;
        
        this->min = center - half_size;
        this->max = center + half_size;
    }
    

    void Box3::setFrom(ptr<Object3D> obj) {
        obj->updateMatrixWorld(true);
        this->makeEmpty();
        
        obj->traverse([&]( ptr<Object3D> node ) {
            if( instance_of(node, Mesh)) {
                ptr<Mesh> mesh = downcast(node, Mesh);
                
                if( mesh->hasGeometry() ) {
                    ptr<Geometry> geom = mesh->getGeometry();
                    
                    for( glm::vec3 vec : geom->getVertices() ) {
                        glm::vec3 trans_vec = glm::vec3(geom->matrixWorld * glm::vec4( vec, 1.0 ) );
                        this->expandByPoint( trans_vec );
                    }
                }
            }
        });
    }
    
    bool Box3::empty() {
        return this->max.x < this->min.x || this->max.y < this->min.y|| this->max.z < this->min.z ;
    }
    
    
    glm::vec3 Box3::center() {
        glm::vec3 result = this->min + this->max;
        result *= 0.5;
        return result;
    }
    
    glm::vec3 Box3::size() {
        return this->max - this->min;
    }
    
    void Box3::expandByPoint(glm::vec3& point) {
        this->min = glm::min( this->min, point );
        this->max = glm::max( this->max, point );
    }
    
    
    void Box3::expandByVector( glm::vec3 vector ) {
        this->min -= vector;
        this->max += vector;
    }
    
    void Box3::expandByScalar( float scalar ) {
        this->min -= scalar;
        this->max += scalar;
    }
    
    bool Box3::contains( glm::vec3 point ) {
        if( point.x < min.x || point.y < min.y || point.z < min.z ||
           point.x > max.x || point.y > max.y || point.z > max.z )
            return false;
        return true;
    }
    
    bool Box3::contains( ptr<Box3> box ) {
        if ( (min.x <= box->min.x) && (min.y <= box->min.y) && (min.z <= box->min.z)  &&
            (box->max.x <= max.x) && (box->max.y <= max.y) && (box->max.z <= max.z) )
            return true;
        return false;
    }
    
    
    void Box3::makeEmpty() {
        min = glm::vec3( MAX_FLOAT, MAX_FLOAT, MAX_FLOAT );
        max = glm::vec3( MIN_FLOAT, MIN_FLOAT, MIN_FLOAT );
    }
    
    
    glm::vec3 Box3::getParameter( glm::vec3 point ) {
        return glm::vec3 ( (point.x - min.x) / (max.x - min.x),
                          (point.y - min.y) / (max.y - min.y),
                          (point.z - min.z) / (max.z - min.z) );
    }
    
    bool Box3::doesIntersect( Box3& box ) {
        if( box.max.x < min.x || box.min.x > max.x ||
           box.max.y < min.y || box.min.y > max.y ||
           box.max.z < min.z || box.min.z > max.z )
            return false;
        return true;
    }
    
    glm::vec3 Box3::clamp( glm::vec3 point ) {
        glm::vec3 result = point;
        result = glm::clamp( result, min, max );
        return result;
    }
    
    float Box3::distanceTo( glm::vec3 point ) {
        glm::vec3 v1 = glm::clamp( point, min, max );
        return glm::length( v1 - point );
    }
    
    
    ptr<Sphere> Box3::getBoundingSphere() {
        return make_shared<Sphere>( this->center(), glm::length(this->size()) * 0.5 );
    }
    
    // FIXME: Test
    ptr<Box3> Box3::applyMatrix( glm::mat4x4& mat ) {
        ptr<Box3> box = Box3::create();
        
        vector<glm::vec4> points {
            glm::vec4(min.x, min.y, min.z, 1),
            glm::vec4(min.x, min.y, max.z, 1),
            glm::vec4(min.x, max.y, min.z, 1),
            glm::vec4(min.x, max.y, max.z, 1),
            glm::vec4(max.x, min.y, min.z, 1),
            glm::vec4(max.x, min.y, max.z, 1),
            glm::vec4(max.x, max.y, min.z, 1),
            glm::vec4(max.x, max.y, max.z, 1),
        };
        
        for( int i = 0; i < 8; i++ )
            points[i]  = mat * points[i];
        
        box->makeEmpty();
        box->setFrom( points );
        
        return box;
    }
    
    void Box3::intersects( Box3& box ) {
        this->min = glm::max(this->min, box.min);
        this->max = glm::min(this->max, box.max);
    }
    
    void Box3::unions( Box3& box ) {
        this->min = glm::min( this->min, box.min );
        this->max = glm::max( this->max, box.max );
    }
    
    void Box3::translate( glm::vec3 offset ) {
        min += offset;
        max += offset;
    }
    
    bool Box3::equals( Box3& box ) {
        return box.min == min && box.max == max;
    }
}