//
//  Object3D.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Object3D.h"
#include "three.h"

using namespace std;

namespace three {
    
    Object3D::Object3D():
        HasID        ( Object3DIDCount++ ),
        visible      ( true ),
        frustumCulled( true ),
        castShadow   ( false ),
        receiveShadow( false ),
        quaternion   ( glm::quat(1.0, 0.0, 0.0, 0.0) ),
        scale        ( glm::vec3(1.0, 1.0, 1.0) ),
        position     ( glm::vec3(0.0, 0.0, 0.0) ),
        up           ( glm::vec3(0.0, 1.0, 0.0) ),
        matrix                  ( glm::mat4(1.0 )),
        matrixWorld             ( glm::mat4(1.0 )),
        matrixAutoUpdate        ( true ),
        matrixWorldNeedsUpdate  ( false ),
        shaderID                ("")
    {
    }
    
    Object3D::~Object3D(){    
    }
    
    
    ptr<Box3> Object3D::computeBoundingBox() {
        ptr<Box3> box = Box3::create();
        box->setFrom(shared_from_this());
        return box;
    }
    
    ptr<Sphere> Object3D::computeBoundingSphere() {
        ptr<Box3> box = Box3::create();
        box->setFrom(shared_from_this());
        return box->getBoundingSphere();
    }
    
    void Object3D::rotate( const glm::vec3 axis, const float angle ) {
        quaternion = glm::rotate( quaternion, angle, axis );
    }
    
    void Object3D::rotateX( const float angle ){
        const static glm::vec3 axis(1.0, 0.0, 0.0);
        rotate( axis, angle );
    }
    
    void Object3D::rotateY( const float angle ) {
        const static glm::vec3 axis(0.0, 1.0, 0.0);
        rotate( axis, angle );
    }
    
    void Object3D::rotateZ( const float angle ) {
        const static glm::vec3 axis(0.0, 0.0, 1.0);
        rotate( axis, angle );
    }
    
    void Object3D::translate( const glm::vec3 offset ) {
        position += offset;
    }
    
    void Object3D::translate( const float x, const float y, const float z ) {
        position.x += x;
        position.y += y;
        position.z += z;
    }
    
    void Object3D::updateMatrix() {
        this->matrix = Math::composeMatrix( this->position, this->quaternion, this->scale );
        this->matrixWorldNeedsUpdate = true;
    }
    
    
    void Object3D::updateMatrixWorld(bool force) {
        if( this->matrixAutoUpdate )
            this->updateMatrix();
        
        if( this-matrixWorldNeedsUpdate || force ) {
            if( this->parent == nullptr )
                this->matrixWorld = this->matrix;
            else
                this->matrixWorld = this->parent->matrixWorld * this->matrix;
            
            matrixWorldNeedsUpdate = false;
            force = true;
        }
        
        for( auto child: children )
            child.second->updateMatrixWorld( force );
    }
    
    void Object3D::add( ptr<Object3D> object ) {
        if( object == shared_from_this() ) {
            cerr << "Unable to add object as its own child" << endl;
            return;
        }
        
        if( object->parent != nullptr )
            object->parent->remove( object );
        
        object->parent = shared_from_this();
        children[object->id] = object;
    }
    
    
    void Object3D::remove( ptr<Object3D> object ) {
        if( children.count(object->id) == 0 )
            return;
        
        object->parent = nullptr;
        children.erase( object->id );
        //FIXME: remove from scene
    }
    
    
    void Object3D::traverse( std::function<void(ptr<Object3D>)> callback ) {
        callback( shared_from_this() );
        
        for( auto entry: children )
            entry.second->traverse( callback );
    }
    
    
    ptr<Object3D> Object3D::getObjectByID( const unsigned int id, bool recursive ) {
        if( children.count( id ) != 0 )
            return children[id];
        
        if( recursive ) {
            for( auto entry: children ) {
                ptr<Object3D> result = entry.second->getObjectByID( id, recursive );
                if( result != nullptr )
                    return result;
            }
        }
        
        return nullptr;
    }
    
    
    vector<ptr<Object3D>> Object3D::getDescendants() {
        vector<ptr<Object3D>> descendants;
        
        for( auto entry: children ){
            descendants.push_back( entry.second );
            
            vector<ptr<Object3D>> grandchildren = entry.second->getDescendants();
            if( !grandchildren.empty() )
                copy( grandchildren.begin(), grandchildren.end(), back_inserter(descendants) );
        }
        
        return descendants;
    }
}