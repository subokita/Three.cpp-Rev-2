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
#include "internal_headers.h"

#include "Object3D.h"

namespace three {
    class Geometry : public Object3D{
        friend class Mesh;
        friend class ParticleSystem;
        friend class Loader;
        
    public:
        static ptr<Geometry> create();
        Geometry();
        virtual ~Geometry();
        
        void addVertex( glm::vec3 vertex );
        void addFace( ptr<Face3> face );
        void addVertices( std::vector<glm::vec3>& vertices );
        void addFaces( std::vector<ptr<Face3>>& faces );
        
        const std::vector<glm::vec3>& getVertices();
        const std::vector<ptr<Face3>>& getFaces();
        
        void mergeVertices();
        void computeFaceNormals();
        void computeVertexNormalsForNonMergedFaces();
        void computeVertexNormals( bool area_weighted );
        
        void setNoOfElements( int size );
        int getNoOfElements() const;
        
    protected:
        virtual void init();
        
    protected:
        std::vector<glm::vec3> vertices;
        std::vector<ptr<Face3>> faces;
        int noOfElements;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Geometry__) */
