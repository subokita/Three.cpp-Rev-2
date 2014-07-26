//
//  ShadowMapPlugin.cpp
//  Three.cpp Rev.2
//
//  Created by Saburo Okita on 24/07/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include "ShadowMapPlugin.h"
#include "three.h"

using namespace std;

namespace three {
    ptr<ShadowMapPlugin> ShadowMapPlugin::create() {
        return make_shared<ShadowMapPlugin>();
    }
    
    ShadowMapPlugin::ShadowMapPlugin() :
        frustum         (Frustum::create()),
        projectionScreen(glm::mat4(1.0)),
        min             (glm::vec3(0.0)),
        max             (glm::vec3(0.0)),
        position        (glm::vec3(0.0)),
        depthShader     (SHADERLIB_DEPTH_RGBA)
    {
    }
    
    ShadowMapPlugin::~ShadowMapPlugin() {
    }
    
    void ShadowMapPlugin::init( ptr<Scene> scene, ptr<Camera> camera ) {
//        renderTarget = RenderTarget::create(GL_FRAMEBUFFER, 0);
        
        depthShader->compileShader();
        Utils::printWithLineNumbers( depthShader->constructVertexShader() );
        Utils::printWithLineNumbers( depthShader->constructFragmentShader() );
        
        descendants = scene->getDescendants();
//        renderTarget->generateFrameBuffer();
    }
    
    void ShadowMapPlugin::setState( ptr<Scene> scene, ptr<Camera> camera ) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glDisable( GL_BLEND );
        glEnable( GL_CULL_FACE );
        glFrontFace( GL_CCW );
        
        glEnable( GL_DEPTH_TEST );
    }
    
    void ShadowMapPlugin::render( ptr<Scene> scene, ptr<Arcball> arcball, ptr<Camera> camera ) {
        vector<ptr<Light>> lights;
        for( ptr<DirectionalLight> light: scene->directionalLights ) {
            if( !light->castShadow )
                continue;
            
            if( light->shadowCascade ) {
                for(int i = 0; i < light->shadowCascadeArray.size(); i++ ) {
                    ptr<VirtualLight> virtual_light;
                    
                    if( light->shadowCascadeArray[i] == nullptr  ) {
                        virtual_light = createVirtualLight( light, i );
                        virtual_light->originalCamera = camera;
                        
                    }
                    else {
                        virtual_light = light->shadowCascadeArray[i];
                    }
                    
                    updateVirtualLight(light, i );
                    lights.push_back( virtual_light );
                }
            }
            else
                lights.push_back( light );
        }
        
        for( ptr<SpotLight> light: scene->spotLights ) {
            if( light->castShadow )
                lights.push_back( light );
        }
        for( ptr<HemisphereLight> light: scene->hemisphereLights ) {
            if( light->castShadow )
                lights.push_back( light );
        }
        for( ptr<PointLight> light: scene->pointLights ) {
            if( light->castShadow )
                lights.push_back( light );
        }
        
        
        for(ptr<Light> light: lights ) {
            if( light->shadowMap == nullptr ) {
                FILTER shadow_filter = FILTER::LINEAR;
                
                if( shadowMapType == SHADOW_MAP::PCF_SOFT )
                    shadow_filter = FILTER::NEAREST_FILTER;
                
                light->shadowMap = RenderTarget::create( GL_FRAMEBUFFER, 0 );
                light->shadowMap->minFilter = shadow_filter;
                light->shadowMap->magFilter = shadow_filter;
                light->shadowMap->format    = PIXEL_FORMAT::RGBA;
                light->shadowMap->width     = light->shadowMapSize[0];
                light->shadowMap->height    = light->shadowMapSize[1];
            }
            
            if( light->shadowCamera == nullptr ) {
                if( instance_of(light, SpotLight)){
                    light->shadowCamera = PerspectiveCamera::create(light->shadowCameraFOV, light->shadowMapSize[0] / light->shadowMapSize[1], light->shadowCameraNear, light->shadowCameraFar );
                    
                }
                else if( instance_of(light, DirectionalLight)){
                    light->shadowCamera = OrthographicCamera::create(light->shadowCameraLeft, light->shadowCameraRight, light->shadowCameraTop, light->shadowCameraBottom, light->shadowCameraNear, light->shadowCameraFar);
                }
                else {
                    cerr << "Unsupported light type for rendering shadow" << endl;
                    continue;
                }
                
                scene->add( light->shadowCamera );
                
                if( scene->autoUpdate )
                    scene->updateMatrixWorld(false);
            }
            
            if( light->shadowCameraVisible ) {
                //FIXME: Do something with cam helper
            }
            
//            if( light->isVirtual && ) {
//                
//            }
            
            
        }
        
        exit(1);
        
        
//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
//        
//        renderTarget->bind();
//        
//        for( auto object: descendants ){
//            object->updateMatrixWorld(false);
//
//            if( instance_of(object, Mesh) == false )
//                continue;
//
//            depthShader->bind();
//            {
//                depthShader->draw(camera, arcball, object, false );
//            }
//            depthShader->unbind();
//        }
    }
    
    
    ptr<VirtualLight> ShadowMapPlugin::createVirtualLight( ptr<Light> light, int cascade ) {
        ptr<VirtualLight> virtual_light = VirtualLight::create();
        
        virtual_light->isVirtual  = true;
        virtual_light->onlyShadow = true;
        virtual_light->castShadow = true;
        
        virtual_light->shadowCameraNear     = light->shadowCameraNear;
        virtual_light->shadowCameraFar      = light->shadowCameraFar;
        virtual_light->shadowCameraTop      = light->shadowCameraTop;
        virtual_light->shadowCameraBottom   = light->shadowCameraBottom;
        virtual_light->shadowCameraLeft     = light->shadowCameraLeft;
        virtual_light->shadowCameraRight    = light->shadowCameraRight;
        
        virtual_light->shadowCameraVisible  = light->shadowCameraVisible;
        virtual_light->shadowDarkness       = light->shadowDarkness;
        
        virtual_light->shadowBias           = light->shadowCascadeBias[cascade];
        virtual_light->shadowMapSize.x      = light->shadowCascadeWidth[cascade];
        virtual_light->shadowMapSize.y      = light->shadowCascadeHeight[cascade];
        
        float near_z = light->shadowCascadeNearZ[cascade];
        float far_z  = light->shadowCascadeFarZ[cascade];
        
        virtual_light->pointsFrustum[0] = glm::vec3(-1.0, -1.0, near_z);
        virtual_light->pointsFrustum[1] = glm::vec3( 1.0, -1.0, near_z);
        virtual_light->pointsFrustum[2] = glm::vec3(-1.0,  1.0, near_z);
        virtual_light->pointsFrustum[3] = glm::vec3( 1.0,  1.0, near_z);
        
        virtual_light->pointsFrustum[4] = glm::vec3(-1.0, -1.0, far_z);
        virtual_light->pointsFrustum[5] = glm::vec3( 1.0, -1.0, far_z);
        virtual_light->pointsFrustum[6] = glm::vec3(-1.0,  1.0, far_z);
        virtual_light->pointsFrustum[7] = glm::vec3( 1.0,  1.0, far_z);
        
        return virtual_light;
    }
    
    void ShadowMapPlugin::updateVirtualLight( ptr<Light> light, int cascade ) {
        ptr<VirtualLight> virtual_light = light->shadowCascadeArray[cascade];
        
        virtual_light->position         = light->position;
        virtual_light->target->position = light->target->position;
        
        virtual_light->lookAt( virtual_light->target->position );
        
        virtual_light->shadowCameraVisible  = light->shadowCameraVisible;
        virtual_light->shadowDarkness       = light->shadowDarkness;
        virtual_light->shadowBias           = light->shadowBias;
        
        float near_z = light->shadowCascadeNearZ[cascade];
        float far_z  = light->shadowCascadeFarZ[cascade];
        
        virtual_light->pointsFrustum[0].z = near_z;
        virtual_light->pointsFrustum[1].z = near_z;
        virtual_light->pointsFrustum[2].z = near_z;
        virtual_light->pointsFrustum[3].z = near_z;
        
        virtual_light->pointsFrustum[4].z = far_z;
        virtual_light->pointsFrustum[5].z = far_z;
        virtual_light->pointsFrustum[6].z = far_z;
        virtual_light->pointsFrustum[7].z = far_z;
    }
}