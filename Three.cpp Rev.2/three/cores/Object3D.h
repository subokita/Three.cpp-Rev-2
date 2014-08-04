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
    
    class Object3D : public HasID, public EventDispatcher, public std::enable_shared_from_this<Object3D> {
    public:
        static ptr<Object3D> create();
        
        Object3D();
        virtual ~Object3D();
        
        ptr<Box3> computeBoundingBox();
        ptr<Sphere> computeBoundingSphere();
        
        void translate( const float x, const float y, const float z );
        void translate( const glm::vec3 offset );
        void rotateX( const float angle );
        void rotateY( const float angle );
        void rotateZ( const float angle );
        void rotate( const glm::vec3 axis, const float angle );
        
        virtual void updateMatrix();
        virtual void updateMatrixWorld(bool force);
        void add( ptr<Object3D> object );
        void remove( ptr<Object3D> object );
        
        void traverse( std::function<void(ptr<Object3D>)> callback );
        ptr<Object3D> getObjectByID( const unsigned int id, bool recursive );
        
        std::vector<ptr<Object3D>> getDescendants();
        void setTarget( glm::vec3 target_pos );
        
        const glm::vec3& getScale();
        const glm::vec3& getPosition();
        
        void setPositionX(float val);
        void setPositionY(float val);
        void setPositionZ(float val);
        
        void setPosition( const glm::vec3 position );
        void setPosition( float x, float y, float z );
        void setScale( float ratio );
        void setScale( const glm::vec3 scale );
        void setScale( float x, float y, float z );
        
        
    public:
        bool visible;
        bool frustumCulled;
        bool castShadow;
        bool receiveShadow;
        
        bool matrixAutoUpdate;
        bool matrixWorldNeedsUpdate;
        
        ptr<Object3D> parent;
        std::map<int, ptr<Object3D>> children;
        
        ptr<Object3D> target;
        glm::quat quaternion;
        glm::vec3 up;
        glm::mat4 matrix;
        glm::mat4 matrixWorld;
        std::string shaderID;
        
    protected:
        glm::vec3 scale;
        glm::vec3 position;

    };
}

#endif /* defined(__Three_cpp_Rev_2__Object3D__) */
