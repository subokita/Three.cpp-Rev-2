//
//  Line3.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Line3.h"
#include "MathUtils.h"

namespace three {
    
    Line3::Line3():
        start( glm::vec3(0, 0, 0) ),
        end  ( glm::vec3(0, 0, 0) )
    {}
    
    Line3::Line3( glm::vec3 start, glm::vec3 end ):
        start( start ),
        end  ( end )
    {}
    
    Line3::~Line3()
    {}
    
    Line3& Line3::set( glm::vec3 start, glm::vec3 end ){
        this->start = start;
        this->end   = end;
        return *this;
    }
    
    
    glm::vec3 Line3::center() {
        glm::vec3 result = start + end;
        result *= 0.5;
        return result;
    }
    
    glm::vec3 Line3::delta() {
        return this->end - this->start;
    }
    
    
    float Line3::distanceSquared() {
        float distance = glm::distance( this->start, this->end);
        return distance * distance;
    }
    
    float Line3::distance() {
        return glm::distance( this->start, this->end);
    }
    
    glm::vec3 Line3::at( float t ) {
        return delta() * t + start;
    }
    
    
    float Line3::closestPointToPointParameter( glm::vec3 point, bool clamp_to_line ) {
        glm::vec3 start_point = point - start;
        glm::vec3 start_end   = end - start;
        
        float start_end_squared = glm::dot( start_end, start_end );
        float start_end_dot_start_point = glm::dot( start_end, start_point );
        float t = start_end_dot_start_point / start_end_squared;
        
        return clamp_to_line ? MathUtils::clamp( t, 0.0, 1.0 ) : t;
    }
    
    glm::vec3 Line3::closestPointToPoint( glm::vec3 point, bool clamp_to_line ) {
        float t = closestPointToPointParameter( point, clamp_to_line );
        return delta() * t + start;
    }
    
    
    Line3& Line3::applyMatrix( glm::mat4x4& mat ) {
        this->start = glm::vec3(mat * glm::vec4(this->start, 1));
        this->end   = glm::vec3(mat * glm::vec4(this->end,   1));
        return *this;
    }
    
    bool Line3::equals( Line3& rhs ) {
        return start == rhs.start && end == rhs.end;
    }
}