//
//  Scene.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Scene.h"
#include "three.h"

using namespace std;

namespace three {
    Scene::Scene() {
        
    }
    
    Scene::~Scene() {
        
    }
    
    PTR(Scene) Scene::create() {
        return shared_ptr<Scene>( new Scene() );
    }
    
    Scene& Scene::addLight( PTR(Light) light ) {

        if( INSTANCE_OF(light, AmbientLight) ) {
            this->ambientLight = DOWNCAST(light, AmbientLight);
        }
        else if( INSTANCE_OF(light, DirectionalLight) ) {
            directionalLights.push_back( DOWNCAST(light, DirectionalLight) );
        }
        else if( INSTANCE_OF(light, PointLight) ) {
            pointLights.push_back( DOWNCAST(light, PointLight) );
        }
        else if( INSTANCE_OF(light, HemisphereLight) ) {
            hemisphereLights.push_back( DOWNCAST(light, HemisphereLight) );
        }
        else if( INSTANCE_OF(light, SpotLight) ) {
            spotLights.push_back( DOWNCAST(light, SpotLight) );
        }
        
        return *this;
    }
    
    Scene& Scene::add( PTR(Object3D) object ) {
        if( INSTANCE_OF(object, Light) )
            this->addLight( DOWNCAST(object, Light) );
        else
            Object3D::add( object );
        
        return *this;
    }
    
    Scene& Scene::add( PTR(Mesh) object ) {
        Object3D::add( object );
        return *this;
    }
}