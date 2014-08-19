//
//  Plane.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Plane.h"
#include "Line3.h"
#include "Sphere.h"

namespace three {
    
    ptr<Plane> Plane::create() {
        return make_shared<Plane>();
    }
    
    ptr<Plane> Plane::create( glm::vec3 normal, float constant ) {
        return make_shared<Plane>( Plane(normal, constant) );
    }
    
    Plane::Plane() :
        normal  ( glm::vec3(0, 0, 0) ),
        constant( 0.0 )
    {}
    
    Plane::Plane( glm::vec3 normal, float constant ):
        normal  ( normal ),
        constant( constant )
    {}
    
    Plane::~Plane(){}
    
    
    Plane& Plane::set( glm::vec3 normal, float constant ){
        this->normal    = normal;
        this->constant  = constant;
        return *this;
    }
    
    Plane& Plane::set( float x, float y, float z, float w ){
        this->normal    = glm::vec3( x, y, z );
        this->constant  = w;
        return *this;
    }
    
    Plane& Plane::setFromNormalAndCoplanarPoint( glm::vec3 normal, glm::vec3 point ){
        this->normal    = normal;
        this->constant  = glm::dot( -point, normal );
        return *this;
    }
    
    Plane& Plane::setFromCoplanarPoints( glm::vec3 a, glm::vec3 b, glm::vec3 c ){
        glm::vec3 normal = glm::normalize(glm::cross( c-b, a-b));
        this->setFromNormalAndCoplanarPoint( normal, a );
        return *this;
    }
    
    Plane& Plane::normalize() {
        float inversed_normal_len = 1.0 / glm::length( this->normal );
        this->normal    *= inversed_normal_len;
        this->constant  *= inversed_normal_len;
        return *this;
    }
    
    Plane& Plane::negate() {
        this->constant  *= -1;
        this->normal    *= -1;
        return *this;
    }
    
    
    float Plane::distanceTo( glm::vec3 point ) {
        return glm::dot( normal, point ) + constant;
    }
    
    float Plane::distanceTo( Sphere& sphere ) {
        return distanceTo( sphere ) - sphere.radius;
    }
    
    
    glm::vec3 Plane::orthoPoint( glm::vec3 point ) {
        float perpendicular_magnitude = distanceTo( point );
        glm::vec3 result = normal * perpendicular_magnitude;
        return result;
    }
    
    
    glm::vec3 Plane::projectPoint( glm::vec3 point ) {
        glm::vec3 result = orthoPoint( point ) - point;
        result *= -1;
        return result;
    }
    
    bool Plane::intersects(Line3& line) {
        float start_sign = distanceTo( line.start );
        float end_sign   = distanceTo( line.end );
        return (start_sign < 0 && end_sign > 0) || (start_sign > 0 && end_sign < 0);
    }
    
    
    glm::vec3 Plane::intersectLine(Line3& line) {
        glm::vec3 result;
        glm::vec3 direction = line.delta();
        
        float cos_angle = glm::dot(normal, direction);
        if( cos_angle == 0.0 ) {
            if( distanceTo( line.start ) == 0.0 )
                return glm::vec3(line.start);
            throw std::runtime_error("Unimplemented case");
        }
        
        float t = -( glm::dot(line.start, normal) + constant ) / cos_angle;
        
        if( t < 0 || t > 1)
            throw std::runtime_error("Unimplemented case");
        
        return direction * t + line.start;
    }
    
    glm::vec3 Plane::coplanarPoint() {
        glm::vec3 result = normal * -constant;
        return result;
    }
    
    
    Plane& Plane::applyMatrix( glm::mat4x4& mat ) {
        glm::mat3x3 normal_mat  = glm::mat3x3(glm::inverseTranspose( mat ));
        glm::vec3 new_normal    = normal_mat * normal;
        glm::vec3 new_coplanar  = glm::vec3(mat * glm::vec4(coplanarPoint(), 1));
        setFromNormalAndCoplanarPoint( new_normal, new_coplanar );
        return *this;
    }
    
    
    Plane& Plane::translate( glm::vec3 offset ) {
        constant = constant - glm::dot( offset, normal );
        return *this;
    }
    
    bool Plane::equals(Plane& rhs ) {
        return normal == rhs.normal && constant == rhs.constant;
    }
}