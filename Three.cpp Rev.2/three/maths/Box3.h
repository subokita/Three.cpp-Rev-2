//
//  Box3.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 14/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Box3__
#define __Three_cpp_Rev_2__Box3__

#include <iostream>
#include "internal_headers.h"
#include "Utils.h"

namespace three {
    class Box3 {
    public:
        static ptr<Box3> create();
        static ptr<Box3> create( glm::vec3 min_vec, glm::vec3 max_vec );
        
        void set( glm::vec3 min_vec, glm::vec3 max_vec );
        void addPoint( glm::vec3 point );
        void setFrom( std::vector<glm::vec4>& points );
        void setFrom( std::vector<glm::vec3>& points );
        void setFrom( glm::vec3 center, glm::vec3 size );
        void setFrom( ptr<Object3D> obj );
        
        void intersects( Box3& box );
        void unions( Box3& box );
        
        void expandByPoint( glm::vec3& point );
        void expandByVector( glm::vec3 vector );
        void expandByScalar( float scalar );
        void makeEmpty();
        
        bool empty();
        bool contains( glm::vec3 point );
        bool contains( ptr<Box3> box );
        bool doesIntersect( Box3& box );
        bool equals( Box3& box );
        
        glm::vec3 center();
        glm::vec3 size();
        glm::vec3 getParameter( glm::vec3 point );
        glm::vec3 clamp( glm::vec3 point );
        float distanceTo( glm::vec3 point );
        
        ptr<Sphere> getBoundingSphere();
        
        ptr<Box3> applyMatrix(glm::mat4x4& mat);
        void translate( glm::vec3 offset );

        Box3();
        Box3( glm::vec3 min_vec, glm::vec3 max_vec );
        ~Box3();
        
    public:
        glm::vec3 min;
        glm::vec3 max;
        
        friend std::ostream &operator <<( std::ostream& os, const Box3& box ) {
            os << Utils::toString( box.min ) << " to " << Utils::toString( box.max );
            return os;
        }
        
    };
}


#endif /* defined(__Three_cpp_Rev_2__Box3__) */
