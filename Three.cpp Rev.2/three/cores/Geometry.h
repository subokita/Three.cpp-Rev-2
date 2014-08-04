//
//  Geometry.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Geometry__
#define __Three_cpp_Rev_2__Geometry__

#include <iostream>
#include <vector>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

#include "Constants.h"
#include "Face3.h"
#include "Object3D.h"

namespace three {
    class Geometry : public Object3D{
        friend class Mesh;
        friend class Box3;
        friend class Loader;
        
    public:
        static ptr<Geometry> create();
        Geometry();
        ~Geometry();
        
        void addVertex( glm::vec3 vertex );
        void addFace( ptr<Face3> face );
        
        void mergeVertices();
        void computeFaceNormals();
        void computeVertexNormalsForNonMergedFaces();
        void computeVertexNormals( bool area_weighted );
        
        const std::vector<glm::vec3>& getVertices();
        
    protected:
        virtual void init();
        

    public:
        
    protected:
        std::vector<glm::vec3> vertices;
        std::vector<ptr<Face3>> faces;
        int noOfElements;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Geometry__) */
