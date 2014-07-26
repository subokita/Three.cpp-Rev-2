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
    Scene::Scene() :
        autoUpdate( true )
    {
    }
    
    Scene::~Scene() {
        
    }
    
    ptr<Scene> Scene::create() {
        return shared_ptr<Scene>( new Scene() );
    }
    
    void Scene::addLight( ptr<Light> light ) {

        if( instance_of(light, AmbientLight) ) {
            this->ambientLight = downcast(light, AmbientLight);
        }
        else if( instance_of(light, DirectionalLight) ) {
            directionalLights.push_back( downcast(light, DirectionalLight) );
        }
        else if( instance_of(light, PointLight) ) {
            pointLights.push_back( downcast(light, PointLight) );
        }
        else if( instance_of(light, HemisphereLight) ) {
            hemisphereLights.push_back( downcast(light, HemisphereLight) );
        }
        else if( instance_of(light, SpotLight) ) {
            spotLights.push_back( downcast(light, SpotLight) );
        }
    }
    
    void Scene::add( ptr<Object3D> object ) {
        if( instance_of(object, Light) )
            this->addLight( downcast(object, Light) );
        else
            Object3D::add( object );
    }
    
    void Scene::add( ptr<Mesh> object ) {
        Object3D::add( object );
    }
}