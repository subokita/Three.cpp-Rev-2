//
//  Object3D.h
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Three_cpp_Rev_2__Object3D__
#define __Three_cpp_Rev_2__Object3D__

#include <iostream>
#include <vector>
#include <map>

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "EventDispatcher.h"
#include "Constants.h"
#include "HasID.h"

namespace three {
    static unsigned int Object3DIDCount = 0;
    
    class Object3D : public HasID,
                     public EventDispatcher,
                     public std::enable_shared_from_this<Object3D> {
    public:
        Object3D();
        virtual ~Object3D();
        
        PTR(Box3) computeBoundingBox();
        PTR(Sphere) computeBoundingSphere();
        
        
        void translate( const float x, const float y, const float z );
        void translate( const glm::vec3 offset );
        void rotateX( const float angle );
        void rotateY( const float angle );
        void rotateZ( const float angle );
        void rotate( const glm::vec3 axis, const float angle );
        
        void updateMatrix();
        void updateMatrixWorld(bool force);
        void add( PTR(Object3D) object );
        void remove( PTR(Object3D) object );
        
        void traverse( std::function<void(PTR(Object3D))> callback );
        PTR(Object3D) getObjectByID( const unsigned int id, bool recursive );
        
        
    public:
        bool visible;
        bool frustumCulled;
        bool castShadow;
        bool receiveShadow;
        
        bool matrixAutoUpdate;
        bool matrixWorldNeedsUpdate;
        
        PTR(Object3D) parent;
        std::map<int, PTR(Object3D)> children;
        
        glm::quat quaternion;
        glm::vec3 scale;
        glm::vec3 position;
        glm::vec3 up;
        glm::mat4 matrix;
        glm::mat4 matrixWorld;
    };
}

#endif /* defined(__Three_cpp_Rev_2__Object3D__) */
