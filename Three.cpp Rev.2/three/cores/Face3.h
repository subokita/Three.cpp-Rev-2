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

#include "Constants.h"
#include "Utils.h"
#include "Color.h"


namespace three {
    class Face3 {
    public:
        static ptr<Face3> create(int a, int b, int c);
        static ptr<Face3> create(int a, int b, int c, glm::vec3 normal, Color color = 0xFFFFFF, int material_index = 0);
        
        virtual ~Face3();
        
        void setVertexNormals( std::initializer_list<glm::vec3> vertex_normals );
        void setVertexUVs    ( std::initializer_list<glm::vec2> vertex_uvs );
        void setVertexColors ( std::initializer_list<Color>     vertex_colors );
        void setVertexColors ( std::initializer_list<glm::vec3> vertex_colors );
        
        const glm::vec3& getFaceNormal();
        void setFaceNormal(glm::vec3 normal);
        void setFaceNormal(float x, float y, float z);
        
    protected:
        Face3();
        Face3( int a, int b, int c );
        Face3( int a, int b, int c, glm::vec3 normal, Color color, int material_index );
        
    /* Data members */
    public:
        int a;
        int b;
        int c;
        glm::vec3 vertexNormals[3];
        glm::vec2 uvs[3];
        glm::vec3 vertexColors[3];
        
    protected:
        glm::vec3              normal;
        Color                  color;
        int                    materialIndex;
        std::vector<glm::vec3> vertexTangents;
        
        
        // FIXME: print the normals, tangents and colors too
        /** Output stream overloading */
        friend std::ostream &operator <<( std::ostream& os, const Face3& obj ) {
            os << "Face3 {" << std::endl;
            os << "\ta            : " << obj.a             << std::endl;
            os << "\tb            : " << obj.b             << std::endl;
            os << "\tc            : " << obj.c             << std::endl;
            os << "\tnormal       : " << Utils::toString( obj.normal ) << std::endl;
            os << "\tvertexNormals: " << Utils::toString( obj.vertexNormals, 3 ) << std::endl;
            os << "\tuvs          : " << Utils::toString( obj.uvs, 3 ) << std::endl;
            os << "\tvertexColros : " << Utils::toString( obj.vertexColors, 3 ) << std::endl;
            os << "\tcolor        : " << obj.color         << std::endl;
            os << "\tmaterialIndex: " << obj.materialIndex << std::endl;
            os << "}";
            return os;
        }
    };
}

#endif /* defined(__Three_cpp_Rev_2__Face3__) */
