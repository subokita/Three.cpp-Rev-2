//
//  Face3.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 08/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Face3__
#define __Three_cpp_Rev_2__Face3__

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Utils.h"
#include "Color.h"
#include "Constants.h"

namespace three {
    class Face3 {
    public:
        static PTR(Face3) create();
        static PTR(Face3) create(int a, int b, int c);
        static PTR(Face3) create(int a, int b, int c, glm::vec3 normal, Color color = 0xFFFFFF, int material_index = 0);
        
        Face3();
        Face3( int a, int b, int c );
        Face3( int a, int b, int c, glm::vec3 normal, Color color, int material_index );
        ~Face3();
        
        
    public:
        /* Data members */
        int                    a;
        int                    b;
        int                    c;
        glm::vec3              normal;
        Color                  color;
        int                    materialIndex;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> vertexNormals;
        std::vector<glm::vec3> vertexTangents;
        std::vector<Color>     vertexColors;
        
        
        // FIXME: print the normals, tangents and colors too
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Face3& obj ) {
            os << "Face3 {" << std::endl;
            os << "\ta            : " << obj.a             << std::endl;
            os << "\tb            : " << obj.b             << std::endl;
            os << "\tc            : " << obj.c             << std::endl;
            os << "\tnormal       : " << Utils::toString( obj.normal ) << std::endl;
            os << "\tuvs          : " << Utils::toString( obj.uvs ) << std::endl;
            os << "\tcolor        : " << obj.color         << std::endl;
            os << "\tmaterialIndex: " << obj.materialIndex << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Face3__) */
