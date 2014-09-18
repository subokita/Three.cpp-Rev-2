//
//  Scene.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 07/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "Scene.h"
#include "three.h"

namespace three {
    Scene::Scene() :
        autoUpdate   ( true ),
        shadowMapType( SHADOW_MAP::BASIC )
    {
    }
    
    Scene::~Scene() {
        
    }
    
    ptr<Scene> Scene::create() {
        return shared_ptr<Scene>( new Scene() );
    }
    
    
    void Scene::setShadowMapType(SHADOW_MAP type) {
        this->shadowMapType = type;
    }
    
    SHADOW_MAP Scene::getShadowMapType() {
        return this->shadowMapType;
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
    
    
    bool Scene::isAutoUpdate() {
        return autoUpdate;
    }
    const ptr<IFog> Scene::getFog(){
        return fog;
    }
    
    const Rect& Scene::getViewportSize() {
        return viewportSize;
    }
    
    void Scene::setViewport( const float x, const float y, const float width, const float height ) {
        viewportSize.x      = x;
        viewportSize.y      = y;
        viewportSize.width  = width;
        viewportSize.height = height;
    }
    
    void Scene::setFog(const ptr<IFog> fog) {
        this->fog = fog;
    }
    
    const ptr<AmbientLight> Scene::getAmbientLight(){
        return ambientLight;
    }
    
    DirectionalLightsArray& Scene::getDirectionalLights() {
        return directionalLights;
    }
    
    HemisphereLightsArray& Scene::getHemisphereLights(){
        return hemisphereLights;
    }
    
    PointLightsArray& Scene::getPointLights(){
        return pointLights;
    }
    
    SpotLightsArray& Scene::getSpotLights(){
        return spotLights;
    }
    
    unsigned int Scene::getShadowCasterCount() {
        return shadowCasterCount = directionalLights.getShadowCasterCount()
                                 + hemisphereLights.getShadowCasterCount()
                                 + spotLights.getShadowCasterCount();
    }
    
    
    void Scene::setShadows(ptr<ShaderLib> shader_lib, bool gamma) {
        if( shadowCasterCount < 1 )
            return;
        
        auto shader = shader_lib->getShader();
        
        vector<glm::mat4> shadow_matrices;
        vector<float> shadow_darknesses;
        vector<float> shadow_biases;
        vector<glm::vec2> shadow_map_sizes;
        vector<ptr<ShadowTexture>> shadow_textures;
        
        int offset  = shader_lib->getConfig()[0]
                    + shader_lib->getConfig()[1]
                    + shader_lib->getConfig()[2]
                    + shader_lib->getConfig()[3];
        
        for( auto entry : directionalLights.getCollection() ) {
            ptr<DirectionalLight> light = entry.second;
            if( !light->visible )
                continue;
            
            if( !light->castShadow )
                continue;
            
            shadow_matrices.push_back   ( light->shadowMatrix );
            shadow_darknesses.push_back ( light->shadowDarkness );
            shadow_biases.push_back     ( light->shadowBias );
            shadow_map_sizes.push_back  ( light->shadowMapSize );
            shadow_textures.push_back   ( light->shadowTexture );
        }
        
        for( auto entry : spotLights.getCollection() ) {
            ptr<SpotLight> light = entry.second;
            if( !light->visible )
                continue;
            
            if( !light->castShadow )
                continue;
            
            shadow_matrices.push_back   ( light->shadowMatrix );
            shadow_darknesses.push_back ( light->shadowDarkness );
            shadow_biases.push_back     ( light->shadowBias );
            shadow_map_sizes.push_back  ( light->shadowMapSize );
            shadow_textures.push_back   ( light->shadowTexture );
        }
        
        shader->setUniform( "shadow_mat",       shadow_matrices );
        shader->setUniform( "shadow_darkness",  shadow_darknesses );
        shader->setUniform( "shadow_bias",      shadow_biases );
        shader->setUniform( "shadow_map_size",  shadow_map_sizes );
        
        vector<GLint> texture_indices;
        for( int i = 0; i < shadow_textures.size(); i++ ) {
            glActiveTexture( GL_TEXTURE0 + offset + i );
            shadow_textures[i]->bind();
            
            texture_indices.push_back( offset + i );
            
            glm::vec2& size = shadow_map_sizes[i];
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0, GL_RGBA, GL_FLOAT, 0 );
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, shadow_textures[i]->wrapR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, shadow_textures[i]->wrapS );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, shadow_textures[i]->wrapT );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, shadow_textures[i]->magFilter );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, shadow_textures[i]->minFilter );
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
        }
        
        shader->setUniform("shadow_map", texture_indices );
    }
    
    void Scene::setUniforms( ptr<ShaderLib> shader_lib, bool gamma ) {
        auto shader = shader_lib->getShader();
        if( fog != nullptr)
            fog->setUniforms(shader_lib, gamma);
        
        if( ambientLight == nullptr )
            ambientLight = AmbientLight::create(0x0);
        
        ambientLight->setUniforms       ( shader_lib, gamma );
        directionalLights.setUniforms   ( shader_lib, gamma );
        hemisphereLights.setUniforms    ( shader_lib, gamma );
        pointLights.setUniforms         ( shader_lib, gamma );
        spotLights.setUniforms          ( shader_lib, gamma );
        
        setShadows( shader_lib, gamma );
    }
}