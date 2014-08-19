//
//  Triangle.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 25/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Triangle.h"
#include "Plane.h"


namespace three {
    ptr<Triangle> Triangle::create(){
        return make_shared<Triangle>();
    }
    
    ptr<Triangle> Triangle::create(glm::vec3 a, glm::vec3 b, glm::vec3 c){
        return make_shared<Triangle>(Triangle(a, b, c));
    }
    
    Triangle::Triangle():
        a(NULL_VEC3),
        b(NULL_VEC3),
        c(NULL_VEC3)
    {}
    
    Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c):
        a( a ),
        b( b ),
        c( c )
    {}
    
    Triangle::~Triangle(){}
    
    glm::vec3 Triangle::normal(){
        glm::vec3 result = glm::cross(c - b, a - b);
        float length = glm::dot(result, result);
        
        if( length > 0.0 )
            return result / sqrtf(length);
        return glm::vec3(0.0);
    }
    
    glm::vec3 Triangle::midpoint(){
        glm::vec3 result = (a + b + c) ;
        result /= 3.0;
        return result;
    }
    
    /**
     * From http://www.blackpawn.com/texts/pointinpoly/default.html
     */
    glm::vec3 Triangle::baryCoordFrom( glm::vec3 point ){
        glm::vec3 v0 = c - a;
        glm::vec3 v1 = b - a;
        glm::vec3 v2 = point - a;
        
        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);
        
        float denom = dot00 * dot11 - dot01 * dot01;
        if( denom == 0.0 )
            return glm::vec3(-2.0, -1.0, -1.0);
        
        float inv_denom = 1.0 / denom;
        float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
        float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
        return glm::vec3(1.0 - u - v, v, u);
    }
    
    bool Triangle::contains( glm::vec3 point ){
        glm::vec3 result = baryCoordFrom( point );
        return (result.x >= 0) && (result.y >= 0) && ((result.x + result.y) <= 1);
    }
    
    void Triangle::setFrom( std::vector<glm::vec3> points, int i0, int i1, int i2 ){
        a = points[i0];
        b = points[i1];
        c = points[i2];
    }
    
    float Triangle::area(){
        return glm::length(glm::cross(c-b, a-b)) * 0.5;
    }
    
    ptr<Plane> Triangle::plane(){
        ptr<Plane> result = Plane::create();
        result->setFromCoplanarPoints(a, b, c);
        return result;
    }
    
    bool Triangle::equals( const Triangle& other ){
        return a == other.a && b == other.b && c == other.c;
    }
}