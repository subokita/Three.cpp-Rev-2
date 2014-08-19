//
//  PolyhedronGeometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 19/08/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__PolyhedronGeometry__
#define __Three_cpp_Rev_2__PolyhedronGeometry__

#include <iostream>
#include "Geometry.h"

namespace three {
    struct TempVec;
    
    class PolyhedronGeometry: public Geometry{
    public:
        PolyhedronGeometry();
        PolyhedronGeometry( std::initializer_list<glm::vec3> vertices, std::initializer_list<int> indices, float radius, int detail );
        virtual ~PolyhedronGeometry();
        
    protected:
        TempVec prepare( glm::vec3 vector );
        void subdivide(ptr<Face3> face, int detail);
        void make( TempVec& v1, TempVec& v2, TempVec& v3 );
        glm::vec2 correctUV( glm::vec2& uv, glm::vec3& vector, float azimuth );
        inline float azimuth( glm::vec3& vector );
        inline float inclination( glm::vec3& vector );
        virtual void init(std::initializer_list<glm::vec3>& vertices, std::initializer_list<int>& indices);
        
    protected:
        float radius;
        int detail;
        std::vector<TempVec> vecs;
        
    };
    
    
    /* I hate this, but dynamic typing in Javascript means that */
    /* three.js could add more data members randomly in the middle of their PolyhedronGeometry class */
    struct TempVec{
        glm::vec3 vertex;
        glm::vec2 uv;
        int index;
        
        TempVec& operator=(const TempVec& rhs) {
            this->vertex = rhs.vertex;
            this->index = rhs.index;
            this->uv = rhs.uv;
            return *this;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__PolyhedronGeometry__) */
