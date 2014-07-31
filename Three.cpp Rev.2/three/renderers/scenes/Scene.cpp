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
    
    
    void Scene::update() {
        directionalLights.update();
        hemisphereLights.update();
        pointLights.update();
        spotLights.update();
    }
    
    void Scene::addLight( ptr<Light> light ) {

        if( instance_of(light, AmbientLight) )
            this->ambientLight = downcast(light, AmbientLight);
        else if( instance_of(light, DirectionalLight) )
            directionalLights.add( light );
        else if( instance_of(light, PointLight) )
            pointLights.add( light );
        else if( instance_of(light, HemisphereLight) )
            hemisphereLights.add( light );
        else if( instance_of(light, SpotLight) )
            spotLights.add( light );
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
    
    
    unsigned int Scene::getShadowCasterCount() {
        return directionalLights.getShadowCasterCount() + hemisphereLights.getShadowCasterCount();
    }
    
    void Scene::setUniforms( ptr<Shader> shader, bool gamma ) {
        if( fog != nullptr)
            fog->setUniforms(shader, gamma);
        
        if( ambientLight == nullptr )
            ambientLight = AmbientLight::create(0x0);
        
        ambientLight->setUniforms       ( shader, gamma );
        directionalLights.setUniforms   ( shader, gamma );
        hemisphereLights.setUniforms    ( shader, gamma );
        pointLights.setUniforms         ( shader, gamma );
        spotLights.setUniforms          ( shader, gamma );
    }
}